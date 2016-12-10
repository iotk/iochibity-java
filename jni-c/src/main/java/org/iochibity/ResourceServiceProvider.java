package org.iochibity;

public abstract class ResourceServiceProvider {
    // C API: OCEntityHandler
    // typedef OCEntityHandlerResult (*OCEntityHandler)
    // (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam);

    private long resourceHandle = 0;

    /*OCEntityHandlerResult*/
    public abstract int service(int flag, DocRequestIn request, CallbackParam callbackParam);
}