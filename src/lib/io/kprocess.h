/*
    This file is part of the KDE libraries

    SPDX-FileCopyrightText: 2007 Oswald Buddenhagen <ossi@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPROCESS_H
#define KPROCESS_H

#include <kcoreaddons_export.h>

#include <QProcess>

class KProcessPrivate;

/**
 * \class KProcess kprocess.h <KProcess>
 *
 * Child process invocation, monitoring and control.
 *
 * This class extends QProcess by some useful functionality, overrides
 * some defaults with saner values and wraps parts of the API into a more
 * accessible one.
 * Only use KProcess if you need the extra features, otherwise QProcess
 * is the preferred way of spawning child processes.
 *
 * @author Oswald Buddenhagen <ossi@kde.org>
 **/
class KCOREADDONS_EXPORT KProcess : public QProcess
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(KProcess)

public:

    /**
     * Modes in which the output channels can be opened.
     */
    enum OutputChannelMode {
        SeparateChannels = QProcess::SeparateChannels,
        /**< Standard output and standard error are handled by KProcess
             as separate channels */
        MergedChannels = QProcess::MergedChannels,
        /**< Standard output and standard error are handled by KProcess
             as one channel */
        ForwardedChannels = QProcess::ForwardedChannels,
        /**< Both standard output and standard error are forwarded
             to the parent process' respective channel */
        OnlyStdoutChannel = QProcess::ForwardedErrorChannel,
        /**< Only standard output is handled; standard error is forwarded */
        OnlyStderrChannel = QProcess::ForwardedOutputChannel
        /**< Only standard error is handled; standard output is forwarded */
    };

    /**
     * Constructor
     */
    explicit KProcess(QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~KProcess() override;

    /**
     * Set how to handle the output channels of the child process.
     *
     * The default is ForwardedChannels, which is unlike in QProcess.
     * Do not request more than you actually handle, as this output is
     * simply lost otherwise.
     *
     * This function must be called before starting the process.
     *
     * @param mode the output channel handling mode
     */
    void setOutputChannelMode(OutputChannelMode mode);

    /**
     * Query how the output channels of the child process are handled.
     *
     * @return the output channel handling mode
     */
    OutputChannelMode outputChannelMode() const;

    /**
     * Set the QIODevice open mode the process will be opened in.
     *
     * This function must be called before starting the process, obviously.
     *
     * @param mode the open mode. Note that this mode is automatically
     *   "reduced" according to the channel modes and redirections.
     *   The default is QIODevice::ReadWrite.
     */
    void setNextOpenMode(QIODevice::OpenMode mode);

    /**
     * Adds the variable @p name to the process' environment.
     *
     * This function must be called before starting the process.
     *
     * @param name the name of the environment variable
     * @param value the new value for the environment variable
     * @param overwrite if @c false and the environment variable is already
     *   set, the old value will be preserved
     */
    void setEnv(const QString &name, const QString &value, bool overwrite = true);

    /**
     * Removes the variable @p name from the process' environment.
     *
     * This function must be called before starting the process.
     *
     * @param name the name of the environment variable
     */
    void unsetEnv(const QString &name);

    /**
     * Empties the process' environment.
     *
     * Note that LD_LIBRARY_PATH/DYLD_LIBRARY_PATH is automatically added
     * on *NIX.
     *
     * This function must be called before starting the process.
     */
    void clearEnvironment();

    /**
     * Set the program and the command line arguments.
     *
     * This function must be called before starting the process, obviously.
     *
     * @param exe the program to execute
     * @param args the command line arguments for the program,
     *   one per list element
     */
    void setProgram(const QString &exe, const QStringList &args = QStringList());

    /**
     * @overload
     *
     * @param argv the program to execute and the command line arguments
     *   for the program, one per list element
     */
    void setProgram(const QStringList &argv);

    /**
     * Append an element to the command line argument list for this process.
     *
     * If no executable is set yet, it will be set instead.
     *
     * For example, doing an "ls -l /usr/local/bin" can be achieved by:
     *  \code
     *  KProcess p;
     *  p << "ls" << "-l" << "/usr/local/bin";
     *  ...
     *  \endcode
     *
     * This function must be called before starting the process, obviously.
     *
     * @param arg the argument to add
     * @return a reference to this KProcess
     */
    KProcess &operator<<(const QString &arg);

    /**
     * @overload
     *
     * @param args the arguments to add
     * @return a reference to this KProcess
     */
    KProcess &operator<<(const QStringList &args);

    /**
     * Clear the program and command line argument list.
     */
    void clearProgram();

    /**
     * Set a command to execute through a shell (a POSIX sh on *NIX
     * and cmd.exe on Windows).
     *
     * Using this for anything but user-supplied commands is usually a bad
     * idea, as the command's syntax depends on the platform.
     * Redirections including pipes, etc. are better handled by the
     * respective functions provided by QProcess.
     *
     * If KProcess determines that the command does not really need a
     * shell, it will transparently execute it without one for performance
     * reasons.
     *
     * This function must be called before starting the process, obviously.
     *
     * @param cmd the command to execute through a shell.
     *   The caller must make sure that all filenames etc. are properly
     *   quoted when passed as argument. Failure to do so often results in
     *   serious security holes. See KShell::quoteArg().
     */
    void setShellCommand(const QString &cmd);

    /**
     * Obtain the currently set program and arguments.
     *
     * @return a list, the first element being the program, the remaining ones
     *  being command line arguments to the program.
     */
    QStringList program() const;

    /**
     * Start the process.
     *
     * @see QProcess::start(const QString &, const QStringList &, OpenMode)
     */
    void start();

    /**
     * Start the process, wait for it to finish, and return the exit code.
     *
     * This method is roughly equivalent to the sequence:
     * @code
     *   start();
     *   waitForFinished(msecs);
     *   return exitCode();
     * @endcode
     *
     * Unlike the other execute() variants this method is not static,
     * so the process can be parametrized properly and talked to.
     *
     * @param msecs time to wait for process to exit before killing it
     * @return -2 if the process could not be started, -1 if it crashed,
     *  otherwise its exit code
     */
    int execute(int msecs = -1);

    /**
     * @overload
     *
     * @param exe the program to execute
     * @param args the command line arguments for the program,
     *   one per list element
     * @param msecs time to wait for process to exit before killing it
     * @return -2 if the process could not be started, -1 if it crashed,
     *  otherwise its exit code
     */
    static int execute(const QString &exe, const QStringList &args = QStringList(), int msecs = -1);

    /**
     * @overload
     *
     * @param argv the program to execute and the command line arguments
     *   for the program, one per list element
     * @param msecs time to wait for process to exit before killing it
     * @return -2 if the process could not be started, -1 if it crashed,
     *  otherwise its exit code
     */
    static int execute(const QStringList &argv, int msecs = -1);

    /**
     * Start the process and detach from it. See QProcess::startDetached()
     * for details.
     *
     * Unlike the other startDetached() variants this method is not static,
     * so the process can be parametrized properly.
     * @note Currently, only the setProgram()/setShellCommand() and
     * setWorkingDirectory() parametrizations are supported.
     *
     * The KProcess object may be re-used immediately after calling this
     * function.
     *
     * @return the PID of the started process or 0 on error
     */
    int startDetached();

    /**
     * @overload
     *
     * @param exe the program to start
     * @param args the command line arguments for the program,
     *   one per list element
     * @return the PID of the started process or 0 on error
     */
    static int startDetached(const QString &exe, const QStringList &args = QStringList());

    /**
     * @overload
     *
     * @param argv the program to start and the command line arguments
     *   for the program, one per list element
     * @return the PID of the started process or 0 on error
     */
    static int startDetached(const QStringList &argv);

    /**
     * Obtain the process' ID as known to the system.
     *
     * Unlike with QProcess::pid(), this is a real PID also on Windows.
     *
     * This function can be called only while the process is running.
     * It cannot be applied to detached processes.
     *
     * @return the process ID
     */
    int pid() const;

protected:
    /**
     * @internal
     */
    KProcess(KProcessPrivate *d, QObject *parent);

    /**
     * @internal
     */
    KProcessPrivate *const d_ptr;

private:
    // hide those
#if QT_DEPRECATED_SINCE(5, 13)
    using QProcess::setReadChannelMode;
    using QProcess::readChannelMode;
#endif
    using QProcess::setProcessChannelMode;
    using QProcess::processChannelMode;
};

#endif

