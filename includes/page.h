#ifndef PAGE_H
#define PAGE_H

#include <QtGui>
#include <QtNetwork>
#include "tag.h"
#include "image.h"
#include "functions.h"

class Image;



class Page : public QObject
{
	Q_OBJECT

	public:
		explicit Page(QMap<QString,QMap<QString,QString> > *sites, QString site, QStringList tags = QStringList(), int page = 1, int limit = 25, QStringList postFiltering = QStringList(), QObject *parent = 0);
		~Page();
		void					load();
		void					loadTags();
		QList<Image*>			images();
		QMap<QString,QString>	site();
		int						imagesCount();
		QUrl					url();
		QString					source();
		QString					website();
		QList<Tag*>				tags();
		QStringList				search();

	public slots:
		void parse(QNetworkReply*);
		void parseTags(QNetworkReply*);
		void abort();
		void abortTags();

	signals:
		void finishedLoading(Page*);
		void finishedLoadingTags(Page*);

	private:
		QMap<QString,QString>	m_site;
		QString					m_format, m_website, m_source;
		QStringList				m_postFiltering, m_search;
		QUrl					m_url, m_urlRegex;
		QList<Image*>			m_images;
		int						m_imagesCount, m_imagesPerPage;
		QList<Tag*>				m_tags;
		QNetworkReply			*m_reply, *m_replyTags;
		bool					m_replyExists, m_replyTagsExists;
};

#endif // PAGE_H