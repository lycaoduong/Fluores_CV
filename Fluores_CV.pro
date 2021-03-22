QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageProcess.cpp \
    flir_spinaker.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    ImageProcess.h \
    mainwindow.h \
    qcustomplot.h \
    AVIRecorder.h \
#        spinnaker/ArrivalEvent.h \
#        spinnaker/BasePtr.h \
#        spinnaker/Camera.h \
#        spinnaker/CameraBase.h \
#        spinnaker/CameraDefs.h \
#        spinnaker/CameraList.h \
#        spinnaker/CameraPtr.h \
#        spinnaker/ChunkData.h \
#        spinnaker/DeviceEvent.h \
#        spinnaker/Event.h \
#        spinnaker/Exception.h \
#        spinnaker/GUI/CameraSelection.h \
#        spinnaker/GUI/CameraSelectionDlg.h \
#        spinnaker/GUI/CameraSelectionWidget.h \
#        spinnaker/GUI/PropertyGrid.h \
#        spinnaker/GUI/PropertyGridDlg.h \
#        spinnaker/GUI/PropertyGridWidget.h \
#        spinnaker/GUI/RapidXML/rapidxml.hpp \
#        spinnaker/GUI/RapidXML/rapidxml_iterators.hpp \
#        spinnaker/GUI/RapidXML/rapidxml_print.hpp \
#        spinnaker/GUI/RapidXML/rapidxml_utils.hpp \
#        spinnaker/GUI/SpinnakerGUI.h \
#        spinnaker/GUI/SpinnakerGUI_GTKmm.h \
#        spinnaker/GUI/SpinnakerGUI_WPF.h \
#        spinnaker/GUI/Viewer.h \
#        spinnaker/GUI/ViewerDlg.h \
#        spinnaker/GUI/ViewerWidget.h \
#        spinnaker/Image.h \
#        spinnaker/ImageEvent.h \
#        spinnaker/ImagePtr.h \
#        spinnaker/ImageStatistics.h \
#        spinnaker/Interface.h \
#        spinnaker/Interface/IArrivalEvent.h \
#        spinnaker/Interface/ICameraBase.h \
#        spinnaker/Interface/ICameraList.h \
#        spinnaker/Interface/IChunkData.h \
#        spinnaker/Interface/IDeviceEvent.h \
#        spinnaker/Interface/IImage.h \
#        spinnaker/Interface/IImageEvent.h \
#        spinnaker/Interface/IImageStatistics.h \
#        spinnaker/Interface/IInterface.h \
#        spinnaker/Interface/IInterfaceEvent.h \
#        spinnaker/Interface/IInterfaceList.h \
#        spinnaker/Interface/ILoggingEvent.h \
#        spinnaker/Interface/IRemovalEvent.h \
#        spinnaker/Interface/ISystem.h \
#        spinnaker/InterfaceEvent.h \
#        spinnaker/InterfaceList.h \
#        spinnaker/InterfacePtr.h \
#        spinnaker/LoggingEvent.h \
#        spinnaker/LoggingEventData.h \
#        spinnaker/LoggingEventDataPtr.h \
#        spinnaker/RemovalEvent.h \
#        spinnaker/SpinGenApi/Autovector.h \
#        spinnaker/SpinGenApi/Base.h \
#        spinnaker/SpinGenApi/BooleanNode.h \
#        spinnaker/SpinGenApi/CategoryNode.h \
#        spinnaker/SpinGenApi/ChunkAdapter.h \
#        spinnaker/SpinGenApi/ChunkAdapterDcam.h \
#        spinnaker/SpinGenApi/ChunkAdapterGEV.h \
#        spinnaker/SpinGenApi/ChunkAdapterGeneric.h \
#        spinnaker/SpinGenApi/ChunkAdapterU3V.h \
#        spinnaker/SpinGenApi/ChunkPort.h \
#        spinnaker/SpinGenApi/CommandNode.h \
#        spinnaker/SpinGenApi/Compatibility.h \
#        spinnaker/SpinGenApi/Container.h \
#        spinnaker/SpinGenApi/Counter.h \
#        spinnaker/SpinGenApi/EnumClasses.h \
#        spinnaker/SpinGenApi/EnumEntryNode.h \
#        spinnaker/SpinGenApi/EnumNode.h \
#        spinnaker/SpinGenApi/EnumNodeT.h \
#        spinnaker/SpinGenApi/EventAdapter.h \
#        spinnaker/SpinGenApi/EventAdapter1394.h \
#        spinnaker/SpinGenApi/EventAdapterGEV.h \
#        spinnaker/SpinGenApi/EventAdapterGeneric.h \
#        spinnaker/SpinGenApi/EventAdapterU3V.h \
#        spinnaker/SpinGenApi/EventPort.h \
#        spinnaker/SpinGenApi/Filestream.h \
#        spinnaker/SpinGenApi/FloatNode.h \
#        spinnaker/SpinGenApi/FloatRegNode.h \
#        spinnaker/SpinGenApi/GCBase.h \
#        spinnaker/SpinGenApi/GCString.h \
#        spinnaker/SpinGenApi/GCStringVector.h \
#        spinnaker/SpinGenApi/GCSynch.h \
#        spinnaker/SpinGenApi/GCTypes.h \
#        spinnaker/SpinGenApi/GCUtilities.h \
#        spinnaker/SpinGenApi/IBoolean.h \
#        spinnaker/SpinGenApi/ICategory.h \
#        spinnaker/SpinGenApi/IChunkPort.h \
#        spinnaker/SpinGenApi/ICommand.h \
#        spinnaker/SpinGenApi/IDestroy.h \
#        spinnaker/SpinGenApi/IDeviceInfo.h \
#        spinnaker/SpinGenApi/IEnumEntry.h \
#        spinnaker/SpinGenApi/IEnumeration.h \
#        spinnaker/SpinGenApi/IEnumerationT.h \
#        spinnaker/SpinGenApi/IFloat.h \
#        spinnaker/SpinGenApi/IInteger.h \
#        spinnaker/SpinGenApi/INode.h \
#        spinnaker/SpinGenApi/INodeMap.h \
#        spinnaker/SpinGenApi/INodeMapDyn.h \
#        spinnaker/SpinGenApi/IPort.h \
#        spinnaker/SpinGenApi/IPortConstruct.h \
#        spinnaker/SpinGenApi/IPortRecorder.h \
#        spinnaker/SpinGenApi/IRegister.h \
#        spinnaker/SpinGenApi/ISelector.h \
#        spinnaker/SpinGenApi/ISelectorDigit.h \
#        spinnaker/SpinGenApi/IString.h \
#        spinnaker/SpinGenApi/IValue.h \
#        spinnaker/SpinGenApi/IntRegNode.h \
#        spinnaker/SpinGenApi/IntegerNode.h \
#        spinnaker/SpinGenApi/Node.h \
#        spinnaker/SpinGenApi/NodeCallback.h \
#        spinnaker/SpinGenApi/NodeCallbackImpl.h \
#        spinnaker/SpinGenApi/NodeMap.h \
#        spinnaker/SpinGenApi/NodeMapFactory.h \
#        spinnaker/SpinGenApi/NodeMapRef.h \
#        spinnaker/SpinGenApi/Persistence.h \
#        spinnaker/SpinGenApi/Pointer.h \
#        spinnaker/SpinGenApi/PortImpl.h \
#        spinnaker/SpinGenApi/PortNode.h \
#        spinnaker/SpinGenApi/PortRecorder.h \
#        spinnaker/SpinGenApi/PortReplay.h \
#        spinnaker/SpinGenApi/PortWriteList.h \
#        spinnaker/SpinGenApi/Reference.h \
#        spinnaker/SpinGenApi/RegisterNode.h \
#        spinnaker/SpinGenApi/RegisterPortImpl.h \
#        spinnaker/SpinGenApi/SelectorSet.h \
#        spinnaker/SpinGenApi/SpinTestCamera.h \
#        spinnaker/SpinGenApi/SpinnakerGenApi.h \
#        spinnaker/SpinGenApi/StringNode.h \
#        spinnaker/SpinGenApi/StringRegNode.h \
#        spinnaker/SpinGenApi/StructPort.h \
#        spinnaker/SpinGenApi/Synch.h \
#        spinnaker/SpinGenApi/Types.h \
#        spinnaker/SpinGenApi/ValueNode.h \
#        spinnaker/SpinVideo.h \
#        spinnaker/SpinVideoDefs.h \
#        spinnaker/Spinnaker.h \
#        spinnaker/SpinnakerDefs.h \
#        spinnaker/SpinnakerPlatform.h \
#        spinnaker/System.h \
#        spinnaker/SystemPtr.h \
#        spinnaker/TransportLayerDefs.h \
#        spinnaker/TransportLayerDevice.h \
#        spinnaker/TransportLayerInterface.h \
#        spinnaker/TransportLayerStream.h \
#        spinnaker/TransportLayerSystem.h \
#        spinnaker/spinc/CameraDefsC.h \
#        spinnaker/spinc/ChunkDataDefC.h \
#        spinnaker/spinc/QuickSpinC.h \
#        spinnaker/spinc/QuickSpinDefsC.h \
#        spinnaker/spinc/SpinVideoC.h \
#        spinnaker/spinc/SpinnakerC.h \
#        spinnaker/spinc/SpinnakerDefsC.h \
#        spinnaker/spinc/SpinnakerGenApiC.h \
#        spinnaker/spinc/SpinnakerGenApiDefsC.h \
#        spinnaker/spinc/SpinnakerPlatformC.h \
#        spinnaker/spinc/TransportLayerDefsC.h \
#        spinnaker/spinc/TransportLayerDeviceC.h \
#        spinnaker/spinc/TransportLayerInterfaceC.h \
#        spinnaker/spinc/TransportLayerStreamC.h \
#        spinnaker/spinc/TransportLayerSystemC.h \

FORMS += \
    mainwindow.ui



INCLUDEPATH += D:\WorkSpace\opencv-build\include #$$PWD/spinnaker

LIBS += D:\WorkSpace\opencv-build\bin\libopencv_core411.dll
LIBS += D:\WorkSpace\opencv-build\bin\libopencv_highgui411.dll
LIBS += D:\WorkSpace\opencv-build\bin\libopencv_imgcodecs411.dll
LIBS += D:\WorkSpace\opencv-build\bin\libopencv_imgproc411.dll
LIBS += D:\WorkSpace\opencv-build\bin\libopencv_features2d411.dll
LIBS += D:\WorkSpace\opencv-build\bin\libopencv_calib3d411.dll
#LIBS += -lSpinnaker

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/Icon.qrc
