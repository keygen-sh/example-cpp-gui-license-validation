#include "main_widget.h"
#include <QtWidgets>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


// A real application should not hard code the account id and license key like this.
// Should instead read them from configuration or ask the user in a dialog.
constexpr auto g_accountId = "1fddcec8-8dd3-4d8d-9b16-215cac0f9b52";
constexpr auto g_licenseKey = "DEMO-DAD877-FCBF82-B83D5A-03E644-V3";


static void _AfterLicenseValidationSuccess(MainWidget *pMainWidget) {
  // Remove all children from pMainWidget.
  {
    QWidget dummyWidget;
    dummyWidget.setLayout(pMainWidget->layout());
  }

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QPushButton("Do some work"));
  pMainWidget->setLayout(mainLayout);
}


static void _AfterLicenseValidationFailure(MainWidget *pMainWidget, QString title, QString detail) {
  // Remove all children from pMainWidget.
  {
    QWidget dummyWidget;
    dummyWidget.setLayout(pMainWidget->layout());
  }

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QLabel(
    QString("License validation failed!\n\nTitle: %1\n\nDetail: %2").arg(title, detail)));
  pMainWidget->setLayout(mainLayout);
}


static void _ValidateLicense(MainWidget *pMainWidget, QVBoxLayout *pLayout) {
  auto manager = new QNetworkAccessManager(pMainWidget);

  QObject::connect(
    manager,
    &QNetworkAccessManager::finished,
    [pMainWidget](QNetworkReply *reply) {
      QString answer = reply->readAll();
      QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());

      if (jsonResponse.object()["meta"].toObject()["valid"].toBool()) {
        _AfterLicenseValidationSuccess(pMainWidget);
      } else {
        QString title, detail;

        if (jsonResponse.object()["errors"].toArray().count() > 0) {
          auto err = jsonResponse.object()["errors"].toArray()[0].toObject();
          title = err["title"].toString();
          detail = err["detail"].toString();
        }

        _AfterLicenseValidationFailure(pMainWidget, title, detail);
      }
    }
  );

  QString postString = QString("{\"meta\":{\"key\":\"%1\"}}").arg(g_licenseKey);
  QByteArray postBytes = postString.toUtf8();
  QNetworkRequest request = QNetworkRequest(QUrl(
    QString("https://api.keygen.sh/v1/accounts/%1/licenses/actions/validate-key").arg(g_accountId)));

  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/vnd.api+json");
  request.setRawHeader("Accept", "application/json");

  // Returns immediately, does not wait for the reply.
  manager->post(request, postBytes);
}


MainWidget::MainWidget() {
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QLabel("Validating license..."));

  setLayout(mainLayout);
  setWindowTitle("Keygen example");
  setMinimumSize(400, 200);

  // Async function, returns immediately.
  _ValidateLicense(this, mainLayout);
}
