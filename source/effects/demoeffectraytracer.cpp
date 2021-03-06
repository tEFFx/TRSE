#include "demoeffectraytracer.h"

DemoEffectRaytracer::DemoEffectRaytracer(QGridLayout* gl) : AbstractDemoEffect (gl)
{
     SetParameters(0);
    m_mc = new CharsetImage(LColorList::C64);

    m_mc->m_currentMode = CharsetImage::FULL_IMAGE;
     m_mc->setMultiColor(true);


//     m_rt.m_globals.m_lights.append(new DirectionalLight(QVector3D(-1,1,0),QVector3D(1,0.7,0)));
    m_rt.m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
//     SetParameters(0);
    m_img = QImage(320,200,QImage::Format_ARGB32);
    //m_img = QImage(160,200,QImage::Format_ARGB32);
  //  m_img = QImage(400,300,QImage::Format_ARGB32);
 //   m_img = QImage(1980,1600,QImage::Format_ARGB32);
    m_rt.m_objects.clear();

 /*   m_rt.m_objects.append(new RayObjectSphere(QVector3D(0,0,0),QVector3D(1,1,1), QVector3D(1,0,1),50,0.5));
    m_rt.m_objects.append(new RayObjectSphere(QVector3D(1.5,2,0),QVector3D(1.0,1.0,1.0), QVector3D(0,1,0),100,0.7));
    m_rt.m_objects.append(new RayObjectSphere(QVector3D(-3.3,-3.3,0),QVector3D(2,2,2), QVector3D(0.1,0.2,1),80,0.6));

    */
//    m_rt.m_objects.append(new RayObjectSphere(QVector3D(0,0,0),QVector3D(2,2,2), QVector3D(1,0.8,0.2),10,0));


    m_rt.m_camera.m_camera = QVector3D(0,0,-5);
    float a = 0.05;
    m_rt.m_globals.m_ambient = QVector3D(a,a,a);

    for (int i=0;i<16; i++) {
        int rw = 8;
        int rx = 16;
        QVector3D pos = QVector3D(rand()%rx-rx/2,rand()%rw-rw/2+1,rand()%rx-rx/2 );
        QVector3D col = QVector3D(rand()%rw,rand()%rw,rand()%rw)/(float)rw;
        int k= rand()%100;
 //       if (i==0) k=10;
   //     if (i==1) k=70;
        float pn = 0.0;
        float ps =3.2;
//        k=45;

        float ref = rand()%100/100.0;
//        k=45;
       if (k>=0 && k<30)
            m_rt.m_objects.append(new RayObjectBox(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
        else if
           (k>=30 && k<=40)
            m_rt.m_objects.append(new RayObjectSphere(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
       else if
          (k>=40 && k<=60)
           m_rt.m_objects.append(new RayObjectCylinder(pos,QVector3D(0.3,0.1,2), Material(col,rand()%100,ref, pn,ps,"")));
        else
            m_rt.m_objects.append(new RayObjectTorus(pos,QVector3D(1,0.3,0), QVector3D(0,1,0),Material(col,rand()%100,ref, pn,ps,"")));

    }

//    m_rt.m_objects.append(new RayObjectTorus(QVector3D(0,0,0),QVector3D(2,1,0), QVector3D(0,1,0), Material(col,rand()%150+100,0.4, 0.0f,19.123,"")));
    m_rt.m_objects.append(new RayObjectPlane(QVector3D(0,-4,0),QVector3D(0,1,0),
                                             Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "brick1.jpg")));





    m_params["camera_x"] = DemoEffectParam("camera_x",0);
}

void DemoEffectRaytracer::run()
{
    while (!m_abort) {
        m_time+=1;
        // m_mc->Clear();
        m_img.fill(QColor(0,0,0,255));
        Init();
        m_timer = QElapsedTimer();
        m_timer.start();
//        m_timer.
//        m_rt.Raytrace(m_img);
        m_rt.Raymarch(m_img);
        m_elapsedTime = m_timer.elapsed();
        ConvertToC64();
      //  m_img.save("test.png");
        emit SignalImageUpdate();
        this->msleep(10);
    }

}

void DemoEffectRaytracer::Save(QString f)
{

}

void DemoEffectRaytracer::Init()
{
   float t = m_time/30;
    static_cast<DirectionalLight*>(m_rt.m_globals.m_lights[0])->m_direction =
            QVector3D(sin(t),1,-cos(t*1.23123)).normalized();

    t = m_time/41.32;
    float r= 11;
    m_rt.m_camera.m_camera =
            QVector3D(sin(t)*r,0.2,cos(t)*r);

    int i=0;
    t = m_time*3.32;

/*    m_rt.m_objects[0]->SetRotation(QVector3D(i+t,-2*t+2*i, 3.12*i+t));
    m_rt.m_objects[1]->SetRotation(QVector3D(i+t,2*t+2*i, 3.12*i+t*2));*/
    for (AbstractRayObject* a: m_rt.m_objects) {
        if (i!=m_rt.m_objects.count()-1)
        a->SetRotation(QVector3D(i*63.1345+t,-2.13*t+2.134*i, 3.12*i+t));
        i++;
    }


    QVector3D rot = QVector3D(0,1,0);
    //m_rt.m_camera.setRotation(rot);
    m_rt.m_camera.m_rotMatrix.setToIdentity();
    //m_rt.m_camera.m_camera = QVector3D(-10,0,10);
    m_rt.m_camera.setupViewmatrix();

    m_rt.m_camera.m_target.setY(sin(m_time/21.34)*4-2);
//    m_rt.m_camera.m_target.setY(-2);
    m_cols.clear();

    m_cols.append(m_params["color_bg"].m_val);
    m_cols.append(m_params["color_mc1"].m_val);
    m_cols.append(m_params["color_main"].m_val); // This one generic
    m_cols.append(m_params["color_mc2"].m_val);

    m_mc->SetColor(m_cols[0],0);
    m_mc->SetColor(m_cols[1],1);
    m_mc->SetColor(m_cols[3],2);
    m_mc->SetColor(m_cols[2],3);

    m_rt.m_camera.m_fov = 90;


    m_rt.m_globals.m_skyScale = sin(m_time/18.12)/3.0 + 0.5;


/*    for (int i=0;i<m_rt.m_objects.count()-1;i++) {
        float t = m_time/12.3 + i*542;
        m_rt.m_objects[i]->m_material.m_reflectivity = 0.1;//sin(t)/2.0+1;
    }*/
}

void DemoEffectRaytracer::SetParameters(int preset)
{
    if (preset==0) {
        m_params["color_bg"] = DemoEffectParam("color_bg",0);
        m_params["color_mc1"] = DemoEffectParam("color_mc1",5);
        m_params["color_mc2"] = DemoEffectParam("color_mc2",6);
        m_params["color_main"] = DemoEffectParam("color_main",2);

    }

}
