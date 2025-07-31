
#include "RoundAnimatedImage.h"
#include <QPainterPath>

RoundAnimatedImage::RoundAnimatedImage(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_movie(nullptr)
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::NoButton);
    setOpacity(1.0);
    setAntialiasing(true);
}

QString RoundAnimatedImage::source() const
{
    return m_source;
}

void RoundAnimatedImage::setSource(const QString &path)
{
    if (m_source == path)
    {
        return;
    }

    // Convert qrc:/ to :/ for Qt resource compatibility
    QString m_source = path;
    if (m_source.startsWith("qrc:/"))
    {
        m_source.replace(0, 4, ":");
    }

    emit sourceChanged();

    if (m_movie)
    {
        m_movie->stop();
        delete m_movie;
    }

    m_movie = new QMovie(m_source);
    if (!m_movie->isValid())
    {
        qWarning() << "Invalid movie source:" << m_source;
        return;
    }
    connect(m_movie, &QMovie::frameChanged, this, &RoundAnimatedImage::onFrameChanged);
    m_movie->start();
}

void RoundAnimatedImage::onFrameChanged(int)
{
    update(); // triggers paint()
}

void RoundAnimatedImage::paint(QPainter *painter)
{
    if (m_movie && m_movie->isValid())
    {
        QImage frame = m_movie->currentImage();
        QRectF bounds = boundingRect();

        // Enable antialiasing for smooth circle
        painter->setRenderHint(QPainter::Antialiasing, true);

        // Create circular clip
        QPainterPath path;
        path.addEllipse(bounds);
        painter->setClipPath(path);

        // Scale the frame to fill the bounding rect, keeping aspect ratio
        QImage scaledFrame = frame.scaled(bounds.size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        // Center the image if aspect ratio does not match
        QPointF topLeft(
            bounds.x() + (bounds.width() - scaledFrame.width()) / 2,
            bounds.y() + (bounds.height() - scaledFrame.height()) / 2
        );

        painter->drawImage(bounds, frame);
    }
}
