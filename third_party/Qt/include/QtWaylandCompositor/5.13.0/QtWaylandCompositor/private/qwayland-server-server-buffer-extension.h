#ifndef QT_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION
#define QT_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION

#include "wayland-server-core.h"
#include <QtWaylandCompositor/private/wayland-server-buffer-extension-server-protocol.h>
#include <QByteArray>
#include <QMultiMap>
#include <QString>

#ifndef WAYLAND_VERSION_CHECK
#define WAYLAND_VERSION_CHECK(major, minor, micro) \
    ((WAYLAND_VERSION_MAJOR > (major)) || \
    (WAYLAND_VERSION_MAJOR == (major) && WAYLAND_VERSION_MINOR > (minor)) || \
    (WAYLAND_VERSION_MAJOR == (major) && WAYLAND_VERSION_MINOR == (minor) && WAYLAND_VERSION_MICRO >= (micro)))
#endif

QT_BEGIN_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wmissing-field-initializers")
QT_WARNING_DISABLE_CLANG("-Wmissing-field-initializers")

#if !defined(Q_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION_EXPORT)
#  if defined(QT_SHARED)
#    define Q_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION_EXPORT Q_DECL_EXPORT
#  else
#    define Q_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION_EXPORT
#  endif
#endif

namespace QtWaylandServer {
    class Q_WAYLAND_SERVER_SERVER_BUFFER_EXTENSION_EXPORT qt_server_buffer
    {
    public:
        qt_server_buffer(struct ::wl_client *client, int id, int version);
        qt_server_buffer(struct ::wl_display *display, int version);
        qt_server_buffer(struct ::wl_resource *resource);
        qt_server_buffer();

        virtual ~qt_server_buffer();

        class Resource
        {
        public:
            Resource() : server_buffer_object(nullptr), handle(nullptr) {}
            virtual ~Resource() {}

            qt_server_buffer *server_buffer_object;
            qt_server_buffer *object() { return server_buffer_object; } 
            struct ::wl_resource *handle;

            struct ::wl_client *client() const { return wl_resource_get_client(handle); }
            int version() const { return wl_resource_get_version(handle); }

            static Resource *fromResource(struct ::wl_resource *resource);
        };

        void init(struct ::wl_client *client, int id, int version);
        void init(struct ::wl_display *display, int version);
        void init(struct ::wl_resource *resource);

        Resource *add(struct ::wl_client *client, int version);
        Resource *add(struct ::wl_client *client, int id, int version);
        Resource *add(struct wl_list *resource_list, struct ::wl_client *client, int id, int version);

        Resource *resource() { return m_resource; }
        const Resource *resource() const { return m_resource; }

        QMultiMap<struct ::wl_client*, Resource*> resourceMap() { return m_resource_map; }
        const QMultiMap<struct ::wl_client*, Resource*> resourceMap() const { return m_resource_map; }

        bool isGlobal() const { return m_global != nullptr; }
        bool isResource() const { return m_resource != nullptr; }

        static const struct ::wl_interface *interface();
        static QByteArray interfaceName() { return interface()->name; }
        static int interfaceVersion() { return interface()->version; }


    protected:
        virtual Resource *server_buffer_allocate();

        virtual void server_buffer_bind_resource(Resource *resource);
        virtual void server_buffer_destroy_resource(Resource *resource);

        virtual void server_buffer_release(Resource *resource);

    private:
        static void bind_func(struct ::wl_client *client, void *data, uint32_t version, uint32_t id);
        static void destroy_func(struct ::wl_resource *client_resource);
        static void display_destroy_func(struct ::wl_listener *listener, void *data);

        Resource *bind(struct ::wl_client *client, uint32_t id, int version);
        Resource *bind(struct ::wl_resource *handle);

        static const struct ::qt_server_buffer_interface m_qt_server_buffer_interface;

        static void handle_release(
            ::wl_client *client,
            struct wl_resource *resource);

        QMultiMap<struct ::wl_client*, Resource*> m_resource_map;
        Resource *m_resource;
        struct ::wl_global *m_global;
        uint32_t m_globalVersion;
        struct DisplayDestroyedListener : ::wl_listener {
            qt_server_buffer *parent;
        };
        DisplayDestroyedListener m_displayDestroyedListener;
    };
}

QT_WARNING_POP
QT_END_NAMESPACE

#endif
