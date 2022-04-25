#ifndef SPIDERPROCESS_H
#define SPIDERPROCESS_H
#include "common.h"
#include "spidercore.h"
#include <QtCore>
enum SpiderProcStage
{
    PROC_SETUP,
    PROC_FINISH
};
class SpiderProcess;
using SpiderProcCallback = std::function<void (SpiderProcStage stage, SpiderProcess *proc)>;
class SpiderProcess : public QObject
{
    Q_OBJECT
private:
    QProcess *m_proc = nullptr;
    QProcessEnvironment *m_env = nullptr;
public:
    explicit SpiderProcess(SpiderProcCallback callback);
    virtual ~SpiderProcess();
    QProcess *proc();
    QProcessEnvironment *env();
    void start();
    bool waitForFinished(int msecs = 30000);
    void startDetached();
};
#endif // SPIDERPROCESS_H
