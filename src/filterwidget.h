#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QObject>
#include <QScrollArea>
#include <QList>

class QCheckBox;
class QButtonGroup;
class QAbstractButton;
class QVBoxLayout;

struct FilterState
{
    QString name;
    bool state;
};

class FilterWidget : public QScrollArea
{
    Q_OBJECT
public:
    FilterWidget();
    void updateFilterSettings();
    void setAllFilters(bool enabled);
    QList<FilterState> getFilterState();
    void readSettings();
    void writeSettings();

public slots:
    void selectAllFilters();
    void deselectAllFilters();

signals:
    void filterStateChanged(QString key, bool state);

private slots:
    void changeFilter(QAbstractButton * button);


private:
    void addCheckBox(const QString& text, QCheckBox* cb, QVBoxLayout* layout);

    // Ansteuerung_Element
    QCheckBox* cbStellBereich;
    // Bahnsteige
    QCheckBox* cbBahnsteigKante;
    QCheckBox* cbBahnsteigZugang;
    // Bahnuebergang
    QCheckBox* cbBueAnlage;
    QCheckBox* cbBueGleisbezogenerGefahrraum;
    // ETCS
    QCheckBox* cbDatenpunkt;
    QCheckBox* cbZubBereichsgrenze;
    QCheckBox* cbZubStreckeneigenschaft;
    // Fahrstrasse
    QCheckBox* cbFstrFahrweg;
    QCheckBox* cbSonstigerPunkt;
    // Geodaten
    QCheckBox* cbGeoKante;
    QCheckBox* cbGeoKnoten;
    QCheckBox* cbGeoPunkt;
    QCheckBox* cbGeschwindigkeitsprofil;
    QCheckBox* cbHoehenpunkt;
    QCheckBox* cbStrecke;
    QCheckBox* cbTechnischerBereich;
    QCheckBox* cbTechnischerPunkt;
    QCheckBox* cbTopKante;
    QCheckBox* cbTopKnoten;
    QCheckBox* cbUeberhoehung;
    // Gleis
    QCheckBox* cbGleisAbschluss;
    QCheckBox* cbGleisAbschnitt;
    QCheckBox* cbGleisArt;
    QCheckBox* cbGleisBaubereich;
    QCheckBox* cbGleisBezeichnung;
    QCheckBox* cbGleisFahrbahn;
    QCheckBox* cbGleisLichtraum;
    QCheckBox* cbGleisSchaltgruppe;
    // Nahbedienbereich
    QCheckBox* cbNbZoneGrenze;
    // Ortung
    QCheckBox* cbFmaElement;
    QCheckBox* cbFmaKomponente;
    QCheckBox* cbZugeinwirkung;
    // PZB
    QCheckBox* cbPzbElement;
    // Signale
    QCheckBox* cbSignal;
    QCheckBox* cbSignalBefestigung;
    // Weichen_und_Gleissperren
    QCheckBox* cbWKrGspKomponente;


    QButtonGroup* buttongroup;
};

#endif // FILTERWIDGET_H
