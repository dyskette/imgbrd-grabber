#include <QFile>
#include "custom-network-access-manager.h"
#include "vendor/qcustomnetworkreply.h"

bool CustomNetworkAccessManager::TestMode = false;


CustomNetworkAccessManager::CustomNetworkAccessManager(QObject *parent)
	: QNetworkAccessManager(parent)
{}

QNetworkReply *CustomNetworkAccessManager::get(const QNetworkRequest &request)
{
	if (CustomNetworkAccessManager::TestMode)
	{
		QString md5 = QString(QCryptographicHash::hash(request.url().toString().toLatin1(), QCryptographicHash::Md5).toHex());
		QString filename = request.url().fileName();
		QString ext = filename.contains('.') ? filename.mid(filename.lastIndexOf('.') + 1) : "html";
		QString host = request.url().host();
		QString path = "tests/resources/pages/" + host + "/" + md5 + "." + ext;

		QFile f(path);
		if (!f.open(QFile::ReadOnly))
		{
			md5 = QString(QCryptographicHash::hash(request.url().toString().toLatin1(), QCryptographicHash::Md5).toHex());
			f.setFileName("tests/resources/pages/" + host + "/" + md5 + "." + ext);

			if (!f.open(QFile::ReadOnly))
			{
				// LCOV_EXCL_START
				if (ext != "jpg" && ext != "png")
				{
					qDebug() << ("Test file not found: " + f.fileName() + " (" + request.url().toString() + ")");
					return nullptr;
				}
				// LCOV_EXCL_STOP

				f.setFileName("tests/resources/image_1x1.png");

				// LCOV_EXCL_START
				if (!f.open(QFile::ReadOnly))
					return nullptr;
				// LCOV_EXCL_STOP
			}
		}

		qDebug() << ("Reply from file: " + request.url().toString() + " -> " + f.fileName());
		QByteArray content = f.readAll();

		QCustomNetworkReply *reply = new QCustomNetworkReply();
		reply->setHttpStatusCode(200, "OK");
		reply->setContentType("text/html");
		reply->setContent(content);

		return reply;
	}

	return QNetworkAccessManager::get(request);
}
