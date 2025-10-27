#ifndef APPS_SETUP_H
#define APPS_SETUP_H

#include <QObject>
#include <QString>

class TimeElapsedTimer;
class CountdownTimer;
class BirthdayTimer;
class MarriedTimer;
class RemoteApi;

class Setup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool setupComplete READ isSetupComplete NOTIFY setupCompleteChanged)
    Q_PROPERTY(int setupStep READ setupStep WRITE setSetupStep NOTIFY setupStepChanged)

  public:
    Setup(MarriedTimer* marriedTimer,
          BirthdayTimer* kuikenTimer,
          CountdownTimer* christmasTimer,
          RemoteApi& remoteApi,
          QObject* parent = nullptr);

    bool isSetupComplete() const;
    int setupStep() const;
    void setSetupStep(int step);

    Q_INVOKABLE void markSetupComplete();
    Q_INVOKABLE void nextSetupStep();
    Q_INVOKABLE void resetSetup();

  signals:
    void setupCompleteChanged();
    void setupStepChanged();

  private:
    void loadProperties();
    void saveProperty(const QString& key, const QVariant& value);
    void registerDevice();

    bool m_setupComplete;
    int m_setupStep;

    // Application references
    MarriedTimer* m_marriedTimer;
    BirthdayTimer* m_kuikenTimer;
    CountdownTimer* m_christmasTimer;
    RemoteApi& m_remoteApi;
};

#endif // APPS_SETUP_H
