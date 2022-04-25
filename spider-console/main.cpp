#include "utf8LogHandler.h"
#include <QtCore>
#include <QtNetwork>
#include <iostream>
static QMap<QString, QString> g_env;
static QByteArray getFromUrl(QString urlString)
{
    QUrl url(urlString);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                         "Gecko) Chrome/100.0.4896.60 Safari/537.36");
    request.setRawHeader("Private-Token", g_env["gitlab_token"].toLatin1());
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    reply->deleteLater();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        return "";
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    return jsonDoc.toJson();
}
static QByteArray postToUrl(QString urlString, QByteArray data)
{
    QUrl url(urlString);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                         "Gecko) Chrome/100.0.4896.60 Safari/537.36");
    request.setRawHeader("Private-Token", g_env["gitlab_token"].toLatin1());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, data);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    reply->deleteLater();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        return "";
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    return jsonDoc.toJson();
}
QJsonDocument allProjects()
{
    QByteArray json = getFromUrl("https://gitlab.com/api/v4/projects?owned=true");
    // qDebug().noquote() << json;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
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
    QByteArray json = getFromUrl(QString("https://gitlab.com/api/v4/projects/%1/repository/files/%2?ref=main")
                                 .arg(projectId)
                                 .arg(QString::fromLatin1(QUrl::toPercentEncoding(path))));
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    // qDebug().noquote() << jsonDoc.toJson();
    return !jsonDoc.toVariant().toMap()["commit_id"].toString().isEmpty();
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
    QCommandLineOption actionOpt(QStringList() << "action", "Action. projects/upload/exists", "action");
    parser.addOption(actionOpt);
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
        // qDebug() << "upload";
        qDebug() << parser.positionalArguments();
        if (parser.positionalArguments().size() < 2)
        {
            qDebug() << "upload takes 2 positional arguments";
            parser.showHelp(-1);
            return -1;
        }
        QString projectName = parser.positionalArguments()[0];
        QString filePath = parser.positionalArguments()[1];
        QString fileName = QFileInfo(filePath).fileName();
        if (parser.positionalArguments().size() >= 3)
        {
            QString directory = parser.positionalArguments()[2];
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
        QString projectId = getProjectId(projectName);
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
        QJsonObject postData;
        postData["branch"] = "main";
        postData["commit_message"] = ".";
        QJsonObject action;
        action["action"] = exists ? "update" : "create";
        action["file_path"] = fileName;
        action["content"] = QString::fromLatin1(base64);
        action["encoding"] = "base64";
        QJsonArray actions;
        actions.append(action);
        postData["actions"] = actions;
        QJsonDocument postDataJsonDoc(postData);
        // qDebug().noquote() << postDataJsonDoc.toJson();
        QString urlString = QString("https://gitlab.com/api/v4/projects/%1/repository/commits").arg(projectId);
        QByteArray result = postToUrl(urlString, postDataJsonDoc.toJson());
        qDebug().noquote() << result;
        return 0;
    }
    else if (action == "exists")
    {
        if (parser.positionalArguments().size() < 2)
        {
            qDebug() << "exists takes 2 positional arguments";
            parser.showHelp(-1);
            return -1;
        }
        QString projectName = parser.positionalArguments()[0];
        QString filePath = parser.positionalArguments()[1];
        QString projectId = getProjectId(projectName);
        bool exists = fileExists(projectId, filePath);
        std::cout << (exists ? "true" : "false") << std::endl;
        return 0;
    }
    parser.showHelp(-1);
    return -1;
    return 0;
}
