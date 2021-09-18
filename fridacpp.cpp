#include "fridacpp.h"

struct GumInvocationListenerProxy {
    GObject parent;
    InvocationListener* proxy;
};

struct GumInvocationListenerProxyClass {
    GObjectClass parent_class;
};

static GType gum_invocation_listener_proxy_get_type();

static void gum_invocation_listener_proxy_iface_init(gpointer g_iface,
    gpointer iface_data);

G_DEFINE_TYPE_EXTENDED(
    GumInvocationListenerProxy, gum_invocation_listener_proxy, G_TYPE_OBJECT, 0,
    G_IMPLEMENT_INTERFACE(GUM_TYPE_INVOCATION_LISTENER,
        gum_invocation_listener_proxy_iface_init))

static void gum_invocation_listener_proxy_init(GumInvocationListenerProxy* self)
{
}

static void gum_invocation_listener_proxy_finalize(GObject* obj)
{
    delete reinterpret_cast<GumInvocationListenerProxy*>(obj)->proxy;

    G_OBJECT_CLASS(gum_invocation_listener_proxy_parent_class)->finalize(obj);
}

static void gum_invocation_listener_proxy_class_init(
    GumInvocationListenerProxyClass* klass)
{
    G_OBJECT_CLASS(klass)->finalize = gum_invocation_listener_proxy_finalize;
}

static void
gum_invocation_listener_proxy_on_enter(GumInvocationListener* listener,
    GumInvocationContext* context)
{
    ((GumInvocationListenerProxy*)listener)->proxy->on_enter(context);
}

static void
gum_invocation_listener_proxy_on_leave(GumInvocationListener* listener,
    GumInvocationContext* context)
{
    ((GumInvocationListenerProxy*)listener)->proxy->on_leave(context);
}

static void gum_invocation_listener_proxy_iface_init(gpointer g_iface,
    gpointer iface_data)
{
    auto* iface = static_cast<GumInvocationListenerInterface*>(g_iface);

    iface->on_enter = gum_invocation_listener_proxy_on_enter;
    iface->on_leave = gum_invocation_listener_proxy_on_leave;
}

InvocationListener::InvocationListener()
    : cproxy((GumInvocationListenerProxy*)g_object_new(
        gum_invocation_listener_proxy_get_type(), nullptr))
{
    cproxy->proxy = this;
}