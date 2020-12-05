#pragma once

#include "McLogMacroGlobal.h"

MC_FORWARD_DECL_CLASS(IMcLoggerRepository)

MC_FORWARD_DECL_PRIVATE_DATA(McLogManager);

class MCLOGQT_EXPORT McLogManager : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McLogManager)
    Q_DISABLE_COPY(McLogManager)
    McLogManager();
public:
    ~McLogManager();
    
    IMcLoggerRepositoryPtr loggerRepository() const noexcept;
    void setLoggerRepository(IMcLoggerRepositoryConstPtrRef val) noexcept;
    
    static McLogManager *instance() noexcept;
    static void installQtMessageHandler() noexcept;
	static void uninstallQtMessageHandler() noexcept;
    
private:
	static void customMessageHandler(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept;
    
    void output(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept;
    
private:
    MC_DECL_PRIVATE(McLogManager)
};
