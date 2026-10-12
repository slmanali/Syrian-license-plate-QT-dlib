#include "capturethread.h"

#include <QtTest>
#include <QTemporaryDir>
#include <QUrl>
#include <opencv2/videoio.hpp>
#include <memory>

// Collect on the test thread, just as the UI receives queued capture signals.
class CaptureResult : public QObject
{
public:
    explicit CaptureResult(CaptureThread *capture)
    {
        connect(capture, &CaptureThread::frameCaptured, this,
                [this](cv::Mat frame) { frames.append(frame); });
        connect(capture, &CaptureThread::captureError, this,
                [this](QString error) { errors.append(error); });
        connect(capture, &QThread::finished, this, [this]() { ++finished; });
    }
    QList<cv::Mat> frames;
    QStringList errors;
    int finished = 0;
};

class CaptureThreadTest : public QObject
{
    Q_OBJECT
private:
    QTemporaryDir directory;
    QString paths[3];
    QString shortPath;

    bool writeVideo(const QString &path, cv::Size size, int channel, int count)
    {
        cv::VideoWriter writer(path.toStdString(), cv::VideoWriter::fourcc('M','J','P','G'), 25, size);
        if (!writer.isOpened())
            return false;
        for (int i = 0; i < count; ++i) {
            cv::Scalar color(10, 10, 10);
            color[channel] = 190 + i % 50;
            writer.write(cv::Mat(size, CV_8UC3, color));
        }
        return true;
    }

    std::unique_ptr<CaptureThread> capture(const QString &path, bool stream, cv::Size size)
    {
        if (stream)
            return std::unique_ptr<CaptureThread>(new CaptureThread(QUrl::fromLocalFile(path).toString(), size));
        return std::unique_ptr<CaptureThread>(new CaptureThread(path.toStdString()));
    }

    void backends()
    {
        QTest::addColumn<bool>("stream");
        QTest::newRow("OpenCV") << false;
        QTest::newRow("LibVLC") << true;
    }

private slots:
    void initTestCase()
    {
        QVERIFY(directory.isValid());
        for (int i = 0; i < 3; ++i) {
            paths[i] = directory.filePath(QString("camera-%1.avi").arg(i));
            QVERIFY(writeVideo(paths[i], cv::Size(96 + 16*i, 64 + 8*i), i, 150));
        }
        shortPath = directory.filePath("short.avi");
        QVERIFY(writeVideo(shortPath, cv::Size(96, 64), 2, 12));
    }

    void independentCameras_data() { backends(); }
    void independentCameras()
    {
        QFETCH(bool, stream);
        // Odd output sizes exercise VLC's padded pitch and explicit resize.
        auto a = capture(paths[0], stream, cv::Size(101, 67));
        auto b = capture(paths[1], stream, cv::Size(123, 79));
        auto c = capture(paths[2], stream, cv::Size(147, 95));
        CaptureResult ra(a.get()), rb(b.get()), rc(c.get());
        QVERIFY(!a->isRunning());
        QVERIFY(!b->isRunning());
        QVERIFY(!c->isRunning());
        a->start(); b->start(); c->start();
        QTRY_VERIFY_WITH_TIMEOUT(ra.frames.size() >= 3 && rb.frames.size() >= 3 && rc.frames.size() >= 3, 10000);
        QVERIFY(ra.errors.isEmpty() && rb.errors.isEmpty() && rc.errors.isEmpty());
        QVERIFY(cv::mean(ra.frames.first())[0] > 170);
        QVERIFY(cv::mean(rb.frames.first())[1] > 170);
        QVERIFY(cv::mean(rc.frames.first())[2] > 170);
        QVERIFY(ra.frames.first().size() == (stream ? cv::Size(101, 67) : cv::Size(96, 64)));
        QVERIFY(rb.frames.first().size() == (stream ? cv::Size(123, 79) : cv::Size(112, 72)));
        QVERIFY(rc.frames.first().size() == (stream ? cv::Size(147, 95) : cv::Size(128, 80)));
        QVERIFY(a->frameSize() == ra.frames.first().size());

        const cv::Mat first = ra.frames.first();
        const cv::Mat snapshot = first.clone();
        a->setPaused(true);
        QTest::qWait(150); // Drain frames that were already queued before pause.
        const int pausedCount = ra.frames.size();
        const int bCount = rb.frames.size();
        const int cCount = rc.frames.size();
        QTest::qWait(200);
        QCOMPARE(ra.frames.size(), pausedCount);
        QVERIFY(rb.frames.size() > bCount);
        QVERIFY(rc.frames.size() > cCount);
        a->togglePause();
        QTRY_VERIFY_WITH_TIMEOUT(ra.frames.size() > pausedCount, 5000);

        b->requestStop();
        QVERIFY(b->wait(3000));
        QTest::qWait(100);
        const int stoppedCount = rb.frames.size();
        const int before = rc.frames.size();
        QTRY_VERIFY_WITH_TIMEOUT(rc.frames.size() > before, 3000);
        QCOMPARE(rb.frames.size(), stoppedCount);
        QCOMPARE(cv::norm(first, snapshot, cv::NORM_INF), 0.0);
        a->requestStop(); c->requestStop();
        QVERIFY(a->wait(3000));
        QVERIFY(c->wait(3000));
    }

    void endOfFile_data() { backends(); }
    void endOfFile()
    {
        QFETCH(bool, stream);
        auto worker = capture(shortPath, stream, cv::Size(96, 64));
        CaptureResult result(worker.get());
        worker->start();
        QTRY_COMPARE_WITH_TIMEOUT(result.finished, 1, 5000);
        QVERIFY(worker->wait(1000));
        QVERIFY(result.errors.isEmpty());
        QVERIFY(!result.frames.isEmpty());
        if (!stream)
            QCOMPARE(result.frames.size(), 12);
        for (const cv::Mat &frame : result.frames)
            QVERIFY(!frame.empty());
    }

    void invalidSource_data() { backends(); }
    void invalidSource()
    {
        QFETCH(bool, stream);
        auto worker = capture(directory.filePath("missing.avi"), stream, cv::Size(96, 64));
        CaptureResult result(worker.get());
        worker->start();
        QTRY_COMPARE_WITH_TIMEOUT(result.finished, 1, 5000);
        QVERIFY(worker->wait(1000));
        QCOMPARE(result.errors.size(), 1);
        QVERIFY(!worker->errorString().isEmpty());
        QVERIFY(result.frames.isEmpty());
    }

    void invalidResolution()
    {
        CaptureThread worker(QUrl::fromLocalFile(paths[0]).toString(), cv::Size(0, 64));
        CaptureResult result(&worker);
        worker.start();
        QTRY_COMPARE_WITH_TIMEOUT(result.finished, 1, 1000);
        QCOMPARE(result.errors.size(), 1);
        QVERIFY(result.frames.isEmpty());
    }

    void stopAndReopen_data() { backends(); }
    void stopAndReopen()
    {
        QFETCH(bool, stream);
        for (int i = 0; i < 5; ++i) {
            auto worker = capture(paths[0], stream, cv::Size(96, 64));
            worker->start();
            worker->requestStop(); // Also covers stopping before run() is scheduled.
            QVERIFY(worker->wait(3000));
        }
        auto worker = capture(paths[0], stream, cv::Size(96, 64));
        CaptureResult result(worker.get());
        worker->setPaused(true);
        worker->start();
        QTest::qWait(150);
        QVERIFY(result.frames.isEmpty());
        worker->setPaused(false);
        QTRY_VERIFY_WITH_TIMEOUT(result.frames.size() >= 3, 5000);
        worker->setPaused(true);
        worker->requestStop();
        QVERIFY(worker->wait(3000));
    }

    void destroyRunningCapture_data() { backends(); }
    void destroyRunningCapture()
    {
        QFETCH(bool, stream);
        auto worker = capture(paths[0], stream, cv::Size(96, 64));
        CaptureResult result(worker.get());
        worker->start();
        QTRY_VERIFY_WITH_TIMEOUT(result.frames.size() >= 2, 5000);
        worker->setPaused(true);
        worker.reset(); // Destructor must wake, join and release the backend.
        QTRY_COMPARE_WITH_TIMEOUT(result.finished, 1, 1000);
    }
};

QTEST_GUILESS_MAIN(CaptureThreadTest)
#include "capturethread_test.moc"
