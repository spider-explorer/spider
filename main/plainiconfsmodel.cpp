#include "plainiconfsmodel.h"
#include <QApplication>
PlainIconFSModel::PlainIconFSModel(QObject *parent) : QFileSystemModel(parent)
{
    QSettings icons_ini(":/vscode_16x16/icons.ini", QSettings::IniFormat);
    qDebug() << icons_ini.allKeys();
    AddLex("", "_");
    AddLex("txt", "_");
    AddLex("csv", "_");
    AddLex("bmp", "_img");
    AddLex("png", "_img");
    AddLex("gif", "_img");
    AddLex("jpg", "_img");
    AddLex("jpeg", "_img");
    AddLex("ico", "_img");
    AddLex("cur", "_img");
    AddLex("icns", "_img");
    AddLex("tif", "_img");
    AddLex("tiff", "_img");
    AddLex("dds", "_img");
    AddLex("heic", "_img");
    AddLex("pspimage", "_img");
    AddLex("tga", "_img");
    AddLex("thm", "_img");
    AddLex("yuv", "_img");
    AddLex("eps", "_img");
    AddLex("3dm", "_img");
    AddLex("3ds", "_img");
    AddLex("max", "_img");
    AddLex("c", "C");
    AddLex("h", "C header");
    AddLex("cpp", "C++");
    AddLex("hpp", "C++ header");
    AddLex("htm", "HTML");
    AddLex("html", "HTML");
    AddLex("css", "CSS");
    AddLex("js", "JavaScript");
    AddLex("json", "JSON");
    AddLex("java", "Java");
    AddLex("sh", "Bash script");
    AddLex("bat", "Batch files");
    AddLex("cmd", "Batch files");
    AddLex("log", "Log files");
    AddLex("md", "Markdown");
    AddLex("xml", "XML");
    AddLex("xsl", "XSLT");
    AddLex("php", "PHP");
    AddLex("py", "Python");
    AddLex("ini", "Ini files");
    AddLex("inf", "Ini files");
    AddLex("rs", "Rust");
    AddLex("vbs", "VBScript");
    AddLex("lua", "Lua");
    AddLex("sql", "SQL");
    AddLex("pas", "Pascal");
    AddLex("yml", "YAML");
    AddLex("yaml", "YAML");
    AddLex("asm", "Assembly");
    AddLex("cs", "C#");
    AddLex("ts", "TypeScript");
    AddLex("rb", "Ruby");
    AddLex("r", "R");
    AddLex("scss", "SCSS");
    AddLex("sass", "Sass");
    AddLex("less", "LESS");
    AddLex("pl", "Perl");
    AddLex("go", "Go");
    AddLex("dart", "Dart");
    AddLex("hs", "Haskell");
    AddLex("erl", "Erlang");
    AddLex("elm", "Elm");
    AddLex("ex", "Elixir");
    AddLex("clj", "Clojure");
    AddLex("coffee", "CoffeeScript");
    AddLex("lisp", "Lisp");
    AddLex("rst", "reStructuredText");
    AddLex("haml", "Haml");
    AddLex("scala", "Scala");
    AddLex("tex", "LaTeX");
    AddLex("tcl", "Tcl");
    AddLex("textile", "Textile");
    AddLex("au3", "AutoIt");
    AddLex("ahk", "AutoHotkey");
    AddLex("diff", "Diff");
    AddLex("cmake", "CMake");
    AddLex("ps1", "PowerShell");
    AddLex("kt", "Kotlin");
    AddLex("styl", "Stylus");
    AddLex("slim", "Slim");
    AddLex("swift", "Swift");
    AddLex("toml", "TOML");
    AddLex("twig", "Twig");
    AddLex("svg", "SVG");
    AddLex("psd", "Photoshop");
    AddLex("d", "D");
    AddLex("f", "Fortran");
    AddLex("for", "Fortran");
    AddLex("f2k", "Fortran");
    AddLex("f90", "Fortran");
    AddLex("f95", "Fortran");
    AddLex("feature", "Gherkin");
    AddLex("gql", "GraphQL");
    AddLex("graphql", "GraphQL");
    AddLex("gradle", "Groovy");
    AddLex("groovy", "Groovy");
    AddLex("tcl", "Tcl");
    AddLex("tk", "Tcl");
    AddLex("tm", "Tcl");
    AddLex("po", "PO");
    AddLex("csproj", "csproj");
    AddLex("fsproj", "fsproj");
    AddLex("vbproj", "vbproj");
    AddLex("vcxproj", "vcxproj");
    AddLex("ai", "AI");
    AddLex("pdf", "PDF");
    AddLex("doc", "Word");
    AddLex("docx", "Word");
    AddLex("rtf", "Word");
    AddLex("odt", "Word");
    AddLex("wpd", "Word");
    AddLex("wps", "Word");
    AddLex("xls", "Excel");
    AddLex("xlsx", "Excel");
    AddLex("xlr", "Excel");
    AddLex("ods", "Excel");
    AddLex("ppt", "PowerPoint");
    AddLex("pps", "PowerPoint");
    AddLex("pptx", "PowerPoint");
    AddLex("zip", "_zip");
    AddLex("zipx", "_zip");
    AddLex("rar", "_zip");
    AddLex("tar", "_zip");
    AddLex("xz", "_zip");
    AddLex("gz", "_zip");
    AddLex("z", "_zip");
    AddLex("7z", "_zip");
    AddLex("deb", "_zip");
    AddLex("pkg", "_zip");
    AddLex("rpm", "_zip");
    AddLex("iso", "_zip");
    AddLex("jar", "_zip");
    AddLex("arj", "_zip");
    AddLex("vcd", "_zip");
    AddLex("cab", "_zip");
    AddLex("cbr", "_zip");
    AddLex("sitx", "_zip");
    AddLex("exe", "_bin");
    AddLex("com", "_bin");
    AddLex("dll", "_bin");
    AddLex("so", "_bin");
    AddLex("ocx", "_bin");
    AddLex("cpl", "_bin");
    AddLex("drv", "_bin");
    AddLex("lnk", "_bin");
    AddLex("sys", "_bin");
    AddLex("gadget", "_bin");
    AddLex("dbg", "_bin");
    AddLex("chm", "_bin");
    AddLex("pyc", "_bin");
    AddLex("o", "_bin");
    AddLex("a", "_bin");
    AddLex("db", "_bin");
    AddLex("dbf", "_bin");
    AddLex("mdb", "_bin");
    AddLex("pdb", "_bin");
    AddLex("accdb", "_bin");
    AddLex("apk", "_bin");
    AddLex("dat", "_bin");
    AddLex("bin", "_bin");
    AddLex("msi", "_bin");
    AddLex("cue", "_bin");
    AddLex("mdf", "_bin");
    AddLex("dmg", "_bin");
    AddLex("toast", "_bin");
    AddLex("wav", "_audio");
    AddLex("mp3", "_audio");
    AddLex("mpa", "_audio");
    AddLex("m4a", "_audio");
    AddLex("m3u", "_audio");
    AddLex("ogg", "_audio");
    AddLex("flac", "_audio");
    AddLex("wma", "_audio");
    AddLex("wpl", "_audio");
    AddLex("aif", "_audio");
    AddLex("iff", "_audio");
    AddLex("cda", "_audio");
    AddLex("mid", "_audio");
    AddLex("midi", "_audio");
    AddLex("mka", "_audio");
    AddLex("s3m", "_audio");
    AddLex("xm", "_audio");
    AddLex("it", "_audio");
    AddLex("ra", "_audio");
    AddLex("mp4", "_video");
    AddLex("mpg", "_video");
    AddLex("mpeg", "_video");
    AddLex("m4v", "_video");
    AddLex("avi", "_video");
    AddLex("mov", "_video");
    AddLex("webm", "_video");
    AddLex("wmv", "_video");
    AddLex("mkv", "_video");
    AddLex("flv", "_video");
    AddLex("ogv", "_video");
    AddLex("3gp", "_video");
    AddLex("3g2", "_video");
    AddLex("asf", "_video");
    AddLex("rm", "_video");
    AddLex("rmvb", "_video");
    AddLex("rv", "_video");
    AddLex("rp", "_video");
    AddLex("viv", "_video");
    AddLex("swf", "_video");
    AddLex("vob", "_video");
    AddLex("fnt", "_font");
    AddLex("fon", "_font");
    AddLex("ttf", "_font");
    AddLex("otf", "_font");
    qDebug() << m_extMap;
}
PlainIconFSModel::~PlainIconFSModel()
{
}
QVariant PlainIconFSModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.model() != this)
        return QVariant();
    if (role == Qt::DecorationRole)
    {
        QFileInfo info(this->filePath(index));
        QString suffix = info.suffix().toLower();
        if (info.isDir())
        {
            return this->iconProvider()->icon(QFileIconProvider::Folder);
        }
        else if (suffix == "pro" || suffix == "pri")
        {
            return QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton);
        }
        else if (suffix == "lpr")
        {
            return QPixmap(":/vscode_16x16/extras/lazarus.png");
        }
        else if (m_extMap.contains(suffix))
        {
            return QPixmap(m_extMap[suffix]);
        }
        else
        {
            // return this->iconProvider()->icon(QFileIconProvider::File);
            return QPixmap(m_extMap[""]);
        }
    }
    return this->QFileSystemModel::data(index, role);
}
#if 0x0
QMap<QString, QString> &PlainIconFSModel::extMap()
{
    return m_extMap;
}
#endif
void PlainIconFSModel::setListWidgetItemIcon(QListWidgetItem *item, QString path)
{
    QFileInfo info(path);
    QString suffix = info.suffix().toLower();
    if (info.isDir())
    {
        item->setIcon(this->iconProvider()->icon(QFileIconProvider::Folder));
    }
    else if (suffix == "pro" || suffix == "pri")
    {
        item->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
    }
    else if (suffix == "lpr")
    {
        item->setIcon(QPixmap(":/vscode_16x16/extras/lazarus.png"));
    }
    else if (m_extMap.contains(suffix))
    {
        item->setIcon(QPixmap(m_extMap[suffix]));
    }
    else
    {
        item->setIcon(QPixmap(m_extMap[""]));
    }
}
void PlainIconFSModel::AddLex(QString ext, QString iconId)
{
    QSettings icons_ini(":/vscode_16x16/icons.ini", QSettings::IniFormat);
    m_extMap[ext] = ":/vscode_16x16/" + icons_ini.value("p/" + iconId).toString();
}
