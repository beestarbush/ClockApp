#include <QQuickPaintedItem>
#include <QMovie>
#include <QPainter>

class RoundAnimatedImage : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    RoundAnimatedImage(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QString source() const;
    void setSource(const QString &path);

signals:
    void sourceChanged();

private slots:
    void onFrameChanged(int frameNumber);

protected:
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) override;

private:
    QMovie *m_movie;
    QString m_source;
};
