// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021  The Ravncore developers
// Copyright (c) @2024 		The YellowDuckieCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/yellowduckiecoin-config.h"

#endif

#include "splashscreen.h"
#include "networkstyle.h"
#include "clientversion.h"
#include "init.h"
#include "util.h"
#include "ui_interface.h"
#include "version.h"

#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#endif

#include <QApplication>
#include <QCloseEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QScreen>

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
#define QTversionPreFiveEleven
#endif

#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

SplashScreen::SplashScreen(const NetworkStyle* networkStyle)
    : QWidget(), curAlignment(0)
{
    int titleVersionVSpace = 25;  // Space between lines
    int titleCopyrightVSpace = 40;

    float fontFactor = 1.0;
    float devicePixelRatio = 1.0;
#if QT_VERSION > 0x050100
    devicePixelRatio = ((QGuiApplication*)QCoreApplication::instance())->devicePixelRatio();
#endif

    // Define text to display
    QString titleText = tr(PACKAGE_NAME);
    QString versionText = QString("Version %1").arg(QString::fromStdString(FormatFullVersion()));
    QString copyrightText = QString::fromUtf8(CopyrightHolders(strprintf("\xc2\xA9 %u-%u ", 2009, COPYRIGHT_YEAR)).c_str());
    QString titleAddText = networkStyle->getTitleAddText();

    QString font = QApplication::font().toString();

    // Create splash screen with increased height
    QSize splashSize(480 * devicePixelRatio, 400 * devicePixelRatio);
    pixmap = QPixmap(splashSize);

    QPainter pixPaint(&pixmap);
    pixPaint.setRenderHint(QPainter::SmoothPixmapTransform);  // Enable smooth scaling

    // Draw gradient background
    QRadialGradient gradient(QPoint(0,0), splashSize.width()/devicePixelRatio);
    gradient.setColorAt(0, QColor("#E8E8E8"));     // Center: bright silver-gray
    gradient.setColorAt(0.5, QColor("#D3D3D3"));   // Middle: light gray
    gradient.setColorAt(1, QColor("#A9A9A9"));     // Edge: darker gray
    QRect rGradient(QPoint(0,0), splashSize);
    pixPaint.fillRect(rGradient, gradient);

    // Center the logo with 20px margin
    int logoSize = 240;  // Logo size
    int margin = 20;    // Set margin to 20px
    int logoY = margin;  // Start logo from top margin
    QRect rectIcon(
        QPoint((splashSize.width() - logoSize * devicePixelRatio) / 2, logoY * devicePixelRatio),
        QSize(logoSize * devicePixelRatio, logoSize * devicePixelRatio)
    );

    // Draw logo with high quality
    const QSize requiredSize(1024, 1024);
    QPixmap icon(networkStyle->getSplashIcon().pixmap(requiredSize));
    pixPaint.setRenderHint(QPainter::SmoothPixmapTransform);  // Enable smooth scaling
    pixPaint.drawPixmap(rectIcon, icon);

    // Calculate text position with margin from logo
    int textStartY = rectIcon.bottom() + margin;  // Start text 20px below logo
    int lineSpacing = 30;  // Space between text lines

    // Draw title text
    pixPaint.setFont(QFont(font, 33 * fontFactor));
    QFontMetrics fm = pixPaint.fontMetrics();
    int titleTextWidth = fm.horizontalAdvance(titleText);
    int titleX = (splashSize.width() - titleTextWidth) / 2;
    pixPaint.setPen(QColor(51, 51, 51));  // Dark gray
    pixPaint.drawText(titleX / devicePixelRatio, textStartY / devicePixelRatio, titleText);

    // Draw version text
    pixPaint.setFont(QFont(font, 15 * fontFactor));
    fm = pixPaint.fontMetrics();
    int versionTextWidth = fm.horizontalAdvance(versionText);
    int versionX = (splashSize.width() - versionTextWidth) / 2;
    pixPaint.setPen(QColor(68, 68, 68));  // Medium gray
    pixPaint.drawText(versionX / devicePixelRatio, 
                     (textStartY + lineSpacing) / devicePixelRatio,
                     versionText);

    // Draw copyright text
    pixPaint.setFont(QFont(font, 10 * fontFactor));
    fm = pixPaint.fontMetrics();
    int copyrightTextWidth = fm.horizontalAdvance(copyrightText);
    int copyrightX = (splashSize.width() - copyrightTextWidth) / 2;
    pixPaint.setPen(QColor(85, 85, 85));  // Light gray
    pixPaint.drawText(copyrightX / devicePixelRatio,
                     (textStartY + lineSpacing * 2) / devicePixelRatio,
                     copyrightText);

    pixPaint.end();

    // Window setup
    setWindowTitle(titleText + " " + titleAddText);
    QRect r(QPoint(), QSize(pixmap.size().width() / devicePixelRatio,
                           pixmap.size().height() / devicePixelRatio));
    resize(r.size());
    setFixedSize(r.size());
    move(QGuiApplication::primaryScreen()->geometry().center() - r.center());

    subscribeToCoreSignals();
    installEventFilter(this);
}

SplashScreen::~SplashScreen()
{
    unsubscribeFromCoreSignals();
}

bool SplashScreen::eventFilter(QObject * obj, QEvent * ev) {
    if (ev->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
        if (keyEvent->text()[0] == 'q') {
            StartShutdown();
        }
    }
    return QObject::eventFilter(obj, ev);
}

void SplashScreen::slotFinish(QWidget *mainWin)
{
    Q_UNUSED(mainWin);

    if (isMinimized())
        showNormal();
    hide();
    deleteLater();
}

static void InitMessage(SplashScreen *splash, const std::string &message)
{
    QMetaObject::invokeMethod(splash, "showMessage",
        Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(message)),
        Q_ARG(int, Qt::AlignBottom | Qt::AlignHCenter),
        Q_ARG(QColor, QColor(255, 255, 255)));
}

static void ShowProgress(SplashScreen *splash, const std::string &title, int nProgress, bool resume_possible)
{
    InitMessage(splash, title + std::string("\n") +
            (resume_possible ? _( "(press q to shutdown and continue later)" )
                             : _( "press q to shutdown" )) +
            strprintf("\n%d", nProgress) + "%");
}

#ifdef ENABLE_WALLET
void SplashScreen::ConnectWallet(CWallet* wallet)
{
    wallet->ShowProgress.connect(boost::bind(ShowProgress, this, _1, _2, false));
    connectedWallets.push_back(wallet);
}
#endif

void SplashScreen::subscribeToCoreSignals()
{
    uiInterface.InitMessage.connect(boost::bind(InitMessage, this, _1));
    uiInterface.ShowProgress.connect(boost::bind(ShowProgress, this, _1, _2, _3));
#ifdef ENABLE_WALLET
    uiInterface.LoadWallet.connect(boost::bind(&SplashScreen::ConnectWallet, this, _1));
#endif
}

void SplashScreen::unsubscribeFromCoreSignals()
{
    uiInterface.InitMessage.disconnect(boost::bind(InitMessage, this, _1));
    uiInterface.ShowProgress.disconnect(boost::bind(ShowProgress, this, _1, _2, _3));
#ifdef ENABLE_WALLET
    for (CWallet* const & pwallet : connectedWallets) {
        pwallet->ShowProgress.disconnect(boost::bind(ShowProgress, this, _1, _2, false));
    }
#endif
}

void SplashScreen::showMessage(const QString &message, int alignment, const QColor &color)
{
    curMessage = message;
    curAlignment = alignment;
    curColor = color;
    update();
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    QRect r = rect().adjusted(5, 5, -5, -5);
    painter.setPen(curColor);
    painter.drawText(r, curAlignment, curMessage);
}

void SplashScreen::closeEvent(QCloseEvent *event)
{
    StartShutdown();
    event->ignore();
} 
