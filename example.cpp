#include "fridacpp.h"
#include <cassert>

int test1()
{
    return 42;
}

class TestClass {
public:
    int test2(int first)
    {
        printf("test2 execute, my this %p, my arg %d\n", this, first);
        return first + 42;
    }
    int test3()
    {
        printf("test3 execute, my this %p\n", this);
        return 0;
    }
};

class HookTest1 : public InvocationListener {

    long thiz = 0;

public:
    void on_enter(GumInvocationContext* context) override
    {
        printf("%s ecx: %x\n", __func__, context->cpu_context->ecx);
        thiz = context->cpu_context->ecx;
        printf("saved this %p\n", thiz);
    }
    void on_leave(GumInvocationContext* context) override
    {
        printf("%s eax: %x, ecx: %x\n", __func__, context->cpu_context->eax, context->cpu_context->ecx);
        gum_invocation_context_replace_return_value(context, (gpointer)100);
        printf("%s eax: %d, ecx: %x\n", __func__, context->cpu_context->eax, context->cpu_context->ecx);

        using Test3type = int (*)();
        auto test3_addr = &TestClass::test3;
        auto test3Type = (Test3type)(*(int*)&test3_addr);

        printf("now thiz %p\n", thiz);

        __asm mov eax, this;
        __asm mov ecx, [eax] HookTest1.thiz;
        __asm call test3_addr;
    }
};

int main()
{
    printf("TESTING\n");
    auto* interceptor = new Intercepter();

    auto* listener = new HookTest1();
    interceptor->attach((gpointer)test1, listener, nullptr);

    auto r = test1();
    assert(r == 100);

    auto x = &TestClass::test2;
    printf("test2 address %p\n", *(long*)&x);
    interceptor->attach((gpointer) * (long*)&x, listener, nullptr);

    TestClass testClass;
    testClass.test2(40);

    return 0;
}
