#include "frida-gum.h"

struct GumInvocationListenerProxy;

class InvocationListener {

public:
    InvocationListener();
    ~InvocationListener() = default;
    virtual void on_enter(GumInvocationContext* ic) = 0;

    virtual void on_leave(GumInvocationContext* ic) = 0;

    GumInvocationListenerProxy* get_handle() { return cproxy; }

private:
    GumInvocationListenerProxy* cproxy;
};

class Intercepter {

    inline static bool init = false;

public:
    Intercepter()
    {
        if (!init) {
            gum_init_embedded();
            init = true;
        }
        _intercepter = gum_interceptor_obtain();
    }

    ~Intercepter() { g_object_unref(_intercepter); }

    bool attach(gpointer function, InvocationListener* listener, gpointer data)
    {
        return gum_interceptor_attach(_intercepter, function, (GumInvocationListener*)listener->get_handle(), data) == GUM_ATTACH_OK;
    }

    bool replace(gpointer oldfun, gpointer newfun, gpointer data)
    {
        return gum_interceptor_replace(_intercepter, oldfun, newfun, data) == GUM_REPLACE_OK;
    }

    void detach(InvocationListener* listener)
    {
        gum_interceptor_detach(_intercepter, (GumInvocationListener*)listener->get_handle());
    }

    void begin_transaction() { gum_interceptor_begin_transaction(_intercepter); }

    void end_transaction() { gum_interceptor_end_transaction(_intercepter); }

private:
    GumInterceptor* _intercepter;
};