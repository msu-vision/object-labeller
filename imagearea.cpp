#include "imagearea.h"

#include <algorithm>
using std::max;
using std::min;

ImageArea::ImageArea(const QString &filename) : activeFrame_(nullptr)
{
    QImage img(filename);
    if (img.isNull()) {
        return;
    }
    img_ = img;
    setMinimumSize(img.width(), img.height());
    setMouseTracking(true);
    update();
}
ImageArea::~ImageArea()
{
}

void ImageArea::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter(this).drawImage(dirtyRect, img_, dirtyRect);
}

void ImageArea::onItemSelect(QListWidgetItem *item)
{
    qDebug() << item->text();
}

void ImageArea::mousePressEvent(QMouseEvent *event)
{
    int curX = event->pos().x();
    int curY = event->pos().y();

    int ind = findFrame(curX, curY);

    if (event->button() == Qt::RightButton && ind != -1) {
        delete frames_[ind];
        frames_.erase(&frames_[ind]);
    } else if (event->button() == Qt::LeftButton) {
        if (ind == -1) {
            activeFrame_ = new Frame(this);
            activeFrame_->setGeometry(curX, curY, 0, 0);
            activeFrame_->show();

            ind = frames_.size();
            frames_.push_back(activeFrame_);

            state = FrameAction::XYResize;
            fromX_ = curX;
            fromY_ = curY;
        } else {
            activeFrame_ = frames_[ind];
            state = computeActionAndSetCursor(curX, curY, frames_[ind]);
        }
    }
    emit frameAdded();
}

void ImageArea::mouseMoveEvent(QMouseEvent *event)
{
    int curX = event->pos().x();
    int curY = event->pos().y();

    if (activeFrame_ == nullptr) {
        int ind = findFrame(curX, curY);

        if (ind != -1) {
            state = computeActionAndSetCursor(curX, curY, frames_[ind]);
        } else {
            setCursor(Qt::ArrowCursor);
        }

    } else {
        QPoint pos = checkBounds(event->pos());
        curX = pos.x();
        curY = pos.y();

        auto frameGeom = activeFrame_->geometry();
        int width = frameGeom.width();
        int height = frameGeom.height();
        int newX = fromX_;
        int newY = fromY_;

        if (state & FrameAction::XResize) {
            width = abs(curX - fromX_ + 1);
            newX = min(fromX_, curX);
        }

        if (state & FrameAction::YResize) {
            height = abs(curY - fromY_ + 1);
            newY = min(fromY_, curY);
        }

        if (state == FrameAction::Move) {
            newX = min(max(curX - posX_ + fromX_, 0), img_.width() - frameGeom.width());
            newY = min(max(curY - posY_ + fromY_, 0), img_.height() - frameGeom.height());
        }

        activeFrame_->setGeometry(newX, newY, width, height);
    }
}

void ImageArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    activeFrame_ = nullptr;
}

int
ImageArea::findFrame(int x, int y)
{
    for (int i = frames_.size() - 1; i >= 0; --i) {
        QRect pos = frames_[i]->geometry();
        if (pos.x() < x && x < pos.x() + pos.width() &&
            pos.y() < y && y < pos.y() + pos.height())
            return i;
    }
    return -1;
}

QPoint
ImageArea::checkBounds(QPoint pos)
{
    if (pos.x() >= img_.width())
        pos.setX(img_.width() - 1);
    else if (pos.x() < 0)
        pos.setX(0);

    if (pos.y() >= img_.height())
        pos.setY(img_.height() - 1);
    else if (pos.y() < 0)
        pos.setY(0);

    return pos;
}

int ImageArea::computeActionAndSetCursor(int x, int y, Frame *frame)
{
    QRect framePos = frame->geometry();

    int frameX = framePos.x();
    int frameY = framePos.y();
    int frameW = framePos.width();
    int frameH = framePos.height();

    int frameMidX = frameX + frameW / 2;
    int frameMidY = frameY + frameH / 2;

    int deltaX = abs(x - frameMidX);
    int deltaY = abs(y - frameMidY);

    int dir = 0;

    if (deltaX > frameW / 4) {
        if (x > frameMidX) {
            qDebug() << "E";
            dir |= Direction::E;
        } else {
            qDebug() << "W";
            dir |= Direction::W;
        }
    }

    if (deltaY > frameH / 3) {
        if (y > frameMidY) {
            qDebug() << "S";
            dir |= Direction::S;
        } else {
            qDebug() << "N";
            dir |= Direction::N;
        }
    }

    QCursor cursor;
    int action;

    switch (dir) {
    case Direction::N:
    case Direction::S:
        action = FrameAction::YResize;
        cursor = Qt::SizeVerCursor;
        break;

    case Direction::W:
    case Direction::E:
        action = FrameAction::XResize;
        cursor = Qt::SizeHorCursor;
        break;

    case Direction::NW:
    case Direction::SE:
        qDebug() << "FDiag";
        action = FrameAction::XYResize;
        cursor = Qt::SizeFDiagCursor;
        break;

    case Direction::NE:
    case Direction::SW:
        qDebug() << "BDiag";
        action = FrameAction::XYResize;
        cursor = Qt::SizeBDiagCursor;
        break;

    default:
        action = FrameAction::Move;
        cursor = Qt::SizeAllCursor;
    };

    if (dir & Direction::N)
        fromY_ = frameY + frameH;
    else
        fromY_ = frameY;

    if (dir & Direction::W)
        fromX_ = frameX + frameW;
    else
        fromX_ = frameX;


    if (action == FrameAction::Move) {
        posX_ = x;
        posY_ = y;
    }
    setCursor(cursor);

    return action;
}
