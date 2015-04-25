class NetworkView : public osgViewer::View
                  , public QObject
{
public:
    NetworkView() : osgViewer::View()
                  , QObject()
    { }

    ~NetworkView();



    /* data */
};
