#ifndef ABSTRACTDEMOEFFECT_H
#define ABSTRACTDEMOEFFECT_H

#include <QImage>
#include <QThread>
#include <QMap>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include "source/LeLib/limage/charsetimage.h"
#include <QElapsedTimer>
class DemoEffectParam {
public:
    QString m_name;
    float m_val;
    DemoEffectParam(QString name, float v) {
        m_val = v;
        m_name = name;
    }
    DemoEffectParam() {

    }
};

class AbstractDemoEffect : public QThread
{
    Q_OBJECT
public:
    QImage m_img;
    float m_time=0;
    bool m_toggleAnim = true;
    bool m_toggleC64 = true;
    QGridLayout* m_gl = nullptr;
    AbstractDemoEffect(QGridLayout* gl);
    bool m_abort = false;
    QMap<QString,DemoEffectParam> m_params;
    CharsetImage* m_mc;
    QVector<int> m_cols;
//    int m_elapsedTime=0;
    QElapsedTimer m_timer;
    int m_elapsedTime;




    virtual void Render(QImage& img) {}

    virtual void SetParameters(int preset) {}
    virtual void Save(QString file);

    virtual void Init() {}


    void ConvertToC64();

    void FillToGUI();
signals:
    void SignalImageUpdate();
public slots:
    void FillFromGUI();

};

#endif // ABSTRACTDEMOEFFECT_H
