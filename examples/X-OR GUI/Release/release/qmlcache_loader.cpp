#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>

static const unsigned char qt_resource_tree[] = {
0,
0,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,
8,0,2,0,0,0,6,0,0,0,2,0,0,0,54,0,
0,0,0,0,1,0,0,0,0,0,0,0,142,0,0,0,
0,0,1,0,0,0,0,0,0,0,20,0,0,0,0,0,
1,0,0,0,0,0,0,0,162,0,0,0,0,0,1,0,
0,0,0,0,0,0,88,0,0,0,0,0,1,0,0,0,
0,0,0,0,114,0,0,0,0,0,1,0,0,0,0};
static const unsigned char qt_resource_names[] = {
0,
1,0,0,0,47,0,47,0,3,0,0,120,60,0,113,0,
109,0,108,0,14,5,8,51,252,0,67,0,111,0,110,0,
110,0,101,0,99,0,116,0,105,0,111,0,110,0,46,0,
113,0,109,0,108,0,14,0,226,20,92,0,78,0,101,0,
116,0,67,0,108,0,97,0,115,0,115,0,105,0,99,0,
46,0,113,0,109,0,108,0,10,9,107,209,188,0,78,0,
101,0,117,0,114,0,111,0,110,0,46,0,113,0,109,0,
108,0,11,12,106,145,211,0,106,0,115,0,83,0,111,0,
117,0,114,0,99,0,101,0,46,0,106,0,115,0,7,4,
199,88,156,0,78,0,101,0,116,0,46,0,113,0,109,0,
108,0,8,8,1,94,92,0,77,0,97,0,105,0,110,0,
46,0,113,0,109,0,108};
static const unsigned char qt_resource_empty_payout[] = { 0, 0, 0, 0, 0 };
QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
QT_END_NAMESPACE
namespace QmlCacheGeneratedCode {
namespace _qml_Main_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _qml_Net_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _qml_jsSource_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _qml_Neuron_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _qml_NetClassic_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _qml_Connection_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/Main.qml"), &QmlCacheGeneratedCode::_qml_Main_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/Net.qml"), &QmlCacheGeneratedCode::_qml_Net_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/jsSource.js"), &QmlCacheGeneratedCode::_qml_jsSource_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/Neuron.qml"), &QmlCacheGeneratedCode::_qml_Neuron_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/NetClassic.qml"), &QmlCacheGeneratedCode::_qml_NetClassic_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qml/Connection.qml"), &QmlCacheGeneratedCode::_qml_Connection_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.version = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
QT_PREPEND_NAMESPACE(qRegisterResourceData)(/*version*/0x01, qt_resource_tree, qt_resource_names, qt_resource_empty_payout);
}
const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qrc)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qrc))
int QT_MANGLE_NAMESPACE(qCleanupResources_qrc)() {
    return 1;
}
