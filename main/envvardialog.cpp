#include "envvardialog.h"
#include "cmdprocess.h"
#include "common.h"
#include "ui_envvardialog.h"
#include "programdb.h"
EnvVarDialog::EnvVarDialog(QMap<QString, QString> &env, QString repo, QWidget *parent)
    : QDialog(parent), m_env(env), m_repo(repo),
      // m_iniFileName(iniFileName),
      ui(new Ui::EnvVarDialog)
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(showContextMenuForListWidget1(const QPoint&)));
    this->reloadVariables();
}
EnvVarDialog::~EnvVarDialog()
{
    delete ui;
}
void EnvVarDialog::reloadVariables()
{
    // QFileInfo file(m_iniFileName);
    // QFileInfo dir(file.absolutePath());
    SpiderSettings settings(m_env, m_repo);
    settings.settings().beginGroup("environmentVariable");
    ui->listWidget->clear();
    auto keys = settings.settings().childKeys();
    for (int i = 0; i < keys.size(); i++)
    {
        ui->listWidget->addItem(keys[i]);
    }
    settings.settings().endGroup();
}
void EnvVarDialog::setVariable(QString name, QString value)
{
    // QFileInfo file(m_iniFileName);
    // QFileInfo dir(file.absolutePath());
    SpiderSettings settings(m_env, m_repo);
    settings.settings().beginGroup("environmentVariable");
    settings.settings().setValue(name, value);
    settings.settings().endGroup();
}
void EnvVarDialog::showContextMenuForListWidget1(const QPoint &pos)
{
    const QPoint localPos = ui->listWidget->mapFrom(this, pos);
    QModelIndex index = ui->listWidget->indexAt(localPos);
    int row = index.row();
    qDebug() << "row=" << index.row();
    if (index.row() < 0)
        return;
    QListWidgetItem *item = ui->listWidget->item(row);
    QMenu contextMenu("Context menu", this);
    QAction *actRemove = new QAction(QString("%1 を削除").arg(item->text()), this);
    QObject::connect(actRemove, &QAction::triggered,
                     [this, item]()
    {
        // QFileInfo file(m_iniFileName);
        // QFileInfo dir(file.absolutePath());
        SpiderSettings settings(m_env, m_repo);
        settings.settings().beginGroup("environmentVariable");
        settings.settings().remove(item->text());
        settings.settings().endGroup();
        this->reloadVariables();
        ui->editVarName->setText("");
        ui->editVarValue->setText("");
        ui->editVarName->setFocus();
    });
    contextMenu.addAction(actRemove);
    contextMenu.exec(mapToGlobal(pos));
}
void EnvVarDialog::on_btnClose_clicked()
{
    this->close();
}
void EnvVarDialog::on_btnAddVar_clicked()
{
    if (ui->editVarName->text().isEmpty())
    {
        QMessageBox::information(this, "確認", "変数名が入力されていません");
        return;
    }
    this->setVariable(ui->editVarName->text().toUpper(), ui->editVarValue->text());
    this->reloadVariables();
    ui->editVarName->setText("");
    ui->editVarValue->setText("");
    ui->editVarName->setFocus();
}
void EnvVarDialog::on_listWidget_currentTextChanged(const QString &currentText)
{
    if (currentText.isEmpty())
        return;
    ui->editVarName->setText(currentText);
    // QFileInfo file(m_iniFileName);
    // QFileInfo dir(file.absolutePath());
    SpiderSettings settings(m_env, m_repo);
    settings.settings().beginGroup("environmentVariable");
    ui->editVarValue->setText(settings.settings().value(currentText).toString());
    ui->editVarValue->setFocus();
    ui->editVarValue->selectAll();
    settings.settings().endGroup();
}
void EnvVarDialog::on_btnTransferToPC_clicked()
{
#if 0x0
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("set -e") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << Qt::flush;
    // QFileInfo file(m_iniFileName);
    // QFileInfo dir(file.absolutePath());
    SpiderSettings settings(m_env, m_repo);
    settings.settings().beginGroup("environmentVariable");
    auto keys = settings.settings().childKeys();
    for (int i = 0; i < keys.size(); i++)
    {
        strm << QString("envset.exe set %1 \"%2\"").arg(keys[i]).arg(settings.settings().value(keys[i]).toString())
             << Qt::endl;
    }
    settings.settings().endGroup();
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(m_env, QString("環境変数をPCに転送"), cmdLines, ".sh");
    proc->run();
#else
    SpiderSettings settings(m_env, m_repo);
    settings.settings().beginGroup("environmentVariable");
    auto keys = settings.settings().childKeys();
    for (int i = 0; i < keys.size(); i++)
    {
        QProcess proc;
        proc.setProgram(ProgramDB().which("rapidee.exe"));
        proc.setArguments(QStringList() << "-S" << keys[i] << settings.settings().value(keys[i]).toString());
        proc.start();
        proc.waitForFinished();
        qDebug() << keys[i] << proc.exitCode();
    }
    settings.settings().endGroup();
    QMessageBox::information(this, "確認", "環境変数をPCに転送しました");
#endif
}
