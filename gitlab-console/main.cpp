#include "utf8LogHandler.h"
#include <QtCore>
#include <iostream>
#include "jnetwork.h"
static QMap<QString, QString> g_env;
QJsonDocument allProjects()
{
    //QByteArray json = getFromUrl("https://gitlab.com/api/v4/projects?owned=true");
    JNetworkManager nm;
    QNetworkRequest request(QUrl("https://gitlab.com/api/v4/projects?owned=true"));
    request.setRawHeader("Private-Token", g_env["gitlab_token"].toLatin1());
    QVariant v = nm.getBatchAsJson(request);
    {
        //qDebug() << "(2a)" << nm.lastReply()->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "(2a)" << nm.batchResult()["httpStatus"];
        //qDebug() << "(2n)" << nm.lastResult();
    }
    // qDebug().noquote() << json;
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(v);
    QJsonArray array = jsonDoc.array();
    QJsonObject result;
    for (int i = 0; i < array.size(); i++)
    {
        QJsonObject object = array[i].toObject();
        QJsonObject resultObject;
        resultObject["id"] = object["id"];
        result[object["path_with_namespace"].toString()] = (resultObject);
    }
    QJsonDocument resultDoc(result);
    return resultDoc;
}
QString getProjectId(QString projectName)
{
    QJsonDocument jsonDoc = allProjects();
    return jsonDoc.toVariant().toMap()[projectName].toMap()["id"].toString();
}
bool fileExists(QString projectId, QString path)
{
    JNetworkManager nm;
    QNetworkRequest request(QString("https://gitlab.com/api/v4/projects/%1/repository/files/%2?ref=main")
                            .arg(projectId)
                            .arg(QString::fromLatin1(QUrl::toPercentEncoding(path))));
    request.setRawHeader("Private-Token", g_env["gitlab_token"].toLatin1());
    QVariantMap result = nm.headBatch(request);
    QVariant v = nm.getBatchAsJson(request);
    {
        //qDebug() << "(3a)" << nm.lastReply()->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "(3a)" << result["httpStatus"];
        //qDebug() << "(3n)" << nm.lastResult();
    }
    int httpStatus = result["httpStatus"].toInt(); //reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        return false;
    }
    return true;
    //return !v.toMap()["commit_id"].toString().isEmpty();
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(utf8LogHandler);
    g_env["dir"] = qApp->applicationDirPath();
    g_env["temp"] = g_env["dir"] + "/temp";
    g_env["prof"] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    g_env["docs"] = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    g_env["msys2"] = g_env["prof"] + "/.software/msys2";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    g_env["path"] = env.value("PATH");
    g_env["gitlab_token"] = env.value("GITLAB_READ_WRITE");
    // qDebug() << "helloハロー©" << g_env["api_key"];
    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption projectOpt(QStringList() << "project", "Project Path", "projectPath");
    parser.addOption(projectOpt);
    QCommandLineOption actionOpt(QStringList() << "action", "Action. projects/upload/exists", "action");
    parser.addOption(actionOpt);
    QCommandLineOption pathOpt(QStringList() << "path", "Upload path", "path");
    parser.addOption(pathOpt);
    if (!parser.parse(qApp->arguments()))
    {
        parser.showHelp(-1);
        return (-1);
    }
    if (!parser.isSet(actionOpt))
    {
        parser.showHelp(-1);
        return -1;
    }
    QString action = parser.value(actionOpt);
    if (action == "projects")
    {
        // qDebug() << actionOpt.description();
        QJsonDocument result = allProjects();
        std::cout << result.toJson().toStdString() << std::endl;
        return 0;
    }
    else if (action == "upload")
    {
        if (!parser.isSet(projectOpt))
        {
            qDebug() << "--project でプロジェクト名を指定してください。";
            parser.showHelp(-1);
            return -1;
        }
        // qDebug() << "upload";
        qDebug() << parser.positionalArguments();
        if (parser.positionalArguments().size() == 0)
        {
            qDebug() << "アップロードするファイル名を指定してください。";
            parser.showHelp(-1);
            return -1;
        }
        QString projectName = parser.value(projectOpt);
        QString projectId = getProjectId(projectName);
        QJsonObject postData;
        QJsonArray actions;
        for (int i = 0; i < parser.positionalArguments().size(); i++)
        {
            QString filePath = parser.positionalArguments()[i];
            QString fileName = QFileInfo(filePath).fileName();
            if (parser.isSet(pathOpt))
            {
                QString directory = parser.value(pathOpt);
                if (directory.endsWith("/"))
                {
                    fileName = directory + fileName;
                }
                else
                {
                    fileName = directory + "/" + fileName;
                }
            }
            qDebug() << projectName << filePath << fileName;
            bool exists = fileExists(projectId, fileName);
            qDebug() << exists;
            // return 0;
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "File open error.";
                return 1;
            }
            QByteArray bytes = file.readAll();
            QByteArray base64 = bytes.toBase64();
            // qDebug().noquote() << base64;
            postData["branch"] = "main";
            postData["commit_message"] = ".";
            QJsonObject action;
            action["action"] = exists ? "update" : "create";
            action["file_path"] = fileName;
            action["content"] = QString::fromLatin1(base64);
            action["encoding"] = "base64";
            actions.append(action);
        }
        postData["actions"] = actions;
        QJsonDocument postDataJsonDoc(postData);
        // qDebug().noquote() << postDataJsonDoc.toJson();
        QString urlString = QString("https://gitlab.com/api/v4/projects/%1/repository/commits").arg(projectId);
        //QByteArray result = postToUrl(urlString, postDataJsonDoc.toJson());
        JNetworkManager nm;
        QNetworkRequest request(urlString);
        request.setRawHeader("Private-Token", g_env["gitlab_token"].toLatin1());
        QVariant result = nm.postBatchJsonRequest(request, postDataJsonDoc.toVariant());
        //if(!result.isValid() || result.isNull())
        {
            //qDebug() << "(1a)" << nm.lastReply()->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug() << "(1n)" << nm.batchResult();
        }
        qDebug().noquote() << result.toJsonDocument().toJson();
        return 0;
    }
    else if (action == "exists")
    {
        if (!parser.isSet(projectOpt))
        {
            qDebug() << "--project でプロジェクト名を指定してください。";
            parser.showHelp(-1);
            return -1;
        }
        QString projectName = parser.value(projectOpt);
        if (parser.positionalArguments().size() < 1)
        {
            qDebug() << "exists takes 1 positional arguments";
            parser.showHelp(-1);
            return -1;
        }
        QString filePath = parser.positionalArguments()[0];
        QString projectId = getProjectId(projectName);
        bool exists = fileExists(projectId, filePath);
        std::cout << (exists ? "true" : "false") << std::endl;
        return 0;
    }
    parser.showHelp(-1);
    return -1;
    return 0;
}
