#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ResourceManager.h"
#include "ocf_exceptions.h"

#include "ocpayload.h"
#include "ocresource.h"
#include "ocstack.h"
#include "octypes.h"

JavaVM* g_jvm;

/* INTERNAL ONLY */

/* FIXME: exceptions when called by c stack */
/* THROW_JNI_EXCEPTION won't work since we're not called from Java */
void populate_request_in(JNIEnv* env, jobject jrequest_in, OCEntityHandlerRequest* crequest_in)
{
    /* printf("populate_request_in ENTRY\n"); */
    jclass k_request_in = (*env)->GetObjectClass(env, jrequest_in);
    if (k_request_in == NULL) {
	    THROW_JNI_EXCEPTION("GetObjectClass failed for RequestIn\n");
	    return;
    }

    /* first, store pointer to the incoming request */
    jfieldID field = (*env)->GetFieldID(env, k_request_in, "localHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for localHandle\n");
    }
    (*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in);

    field = (*env)->GetFieldID(env, k_request_in, "remoteHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for remoteHandle\n");
    } else {
	printf("c request remote handle: %ld\n", (long)crequest_in->requestHandle);
	(*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->requestHandle);
    }
    /* then do the rest */
    field = (*env)->GetFieldID(env, k_request_in, "resourceHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for resourceHandle\n");
    } else {
	/* printf("OCResourceHandle (in c): %ld\n", (long)crequest_in->resource); */
	(*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->resource);
    }

    /* method */
    field = (*env)->GetFieldID(env, k_request_in, "method", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'method' fld\n");
    } else {
	/* printf("method in c: %d\n", crequest_in->method); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->method);
    }
    /* OCDevAddr */
    jfieldID dev_addr_field = (*env)->GetFieldID(env, k_request_in,
						 "deviceAddress", "Lorg/iochibity/DeviceAddress;");
    if (dev_addr_field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for deviceAddress\n");
    }
    /* construct a new DeviceAddress object to insert into RequestIn obj */
    jclass k_devaddr = (*env)->FindClass(env, "Lorg/iochibity/DeviceAddress;");
    if (k_devaddr == NULL) {
	THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/DeviceAddress\n");
    }
    jmethodID dev_addr_ctor = (*env)->GetMethodID(env, k_devaddr, "<init>", "()V");
    if (dev_addr_ctor == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for ctor of DeviceAddress\n");
    }
    jobject jdevice_address = (*env)->NewObject(env, k_devaddr, dev_addr_ctor);

    /* OCDevAddr.adapter */
    field = (*env)->GetFieldID(env, k_devaddr, "adapter", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for adapter\n");
    } else {
	/* printf("c adapter: %d\n", crequest_in->devAddr.adapter); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.adapter);
    }
    /* OCDevAddr.flags */
    field = (*env)->GetFieldID(env, k_devaddr, "flags", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 0 fld\n");
    } else {
	/* printf("c flags: 0x%X\n", crequest_in->devAddr.flags); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.flags);
    }
    /* OCDevAddr.port */
    field = (*env)->GetFieldID(env, k_devaddr, "port", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for port fld\n");
    } else {
	/* printf("c port: %d\n", crequest_in->devAddr.port); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.port);
    }
    /* OCDevAddr.addr */
    field = (*env)->GetFieldID(env, k_devaddr, "address", "Ljava/lang/String;");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for address fld\n");
    } else {
	/* printf("c address: %s\n", crequest_in->devAddr.addr); */
	jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.addr);
	(*env)->SetObjectField(env, jdevice_address, field, js);
    }
    /* OCDevAddr.ifindex */
    field = (*env)->GetFieldID(env, k_devaddr, "ifindex", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for ifindex fld\n");
    } else {
	/* printf("c ifindex: %d\n", crequest_in->devAddr.ifindex); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.ifindex);
    }
    /* FIXME */
    /* /\* OCDevAddr.routeData *\/ */
    /* if (crequest_in->devAddr.routeData) { */
    /* 	field = (*env)->GetFieldID(env, k_devaddr, "routeData", "Ljava/lang/String;"); */
    /* 	if (field == NULL) { */
    /* 	    printf("Failed to get dev addr routeData fld\n"); */
    /* 	} else { */
    /* 	    printf("c route data: %s\n", crequest_in->devAddr.routeData); */
    /* 	    jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.routeData); */
    /* 	    (*env)->SetObjectField(env, jdevice_address, field, js); */
    /* 	} */
    /* } */
    /* now set deviceAddress field in RequestIn object */
    (*env)->SetObjectField(env, jrequest_in, dev_addr_field, jdevice_address);

    /* query */
    field = (*env)->GetFieldID(env, k_request_in, "query", "Ljava/lang/String;");
    if (field == NULL) { /* make sure we got the field */
	THROW_JNI_EXCEPTION("GetFieldID failed for query fld\n");
    } else {
	/* printf("query in c: '%s'\n", crequest_in->query); */
	jstring js = (*env)->NewStringUTF(env, crequest_in->query);
	(*env)->SetObjectField(env, jrequest_in, field, js);
    }

    /* observation info */
    field = (*env)->GetFieldID(env, k_request_in, "observeAction", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for observeAction fld\n");
    } else {
	/* printf("c observeAction: %d\n", crequest_in->obsInfo.action); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.action);
    }
    field = (*env)->GetFieldID(env, k_request_in, "observeId", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for observeId fld\n");
    } else {
	/* printf("c observation id: %d\n", crequest_in->obsInfo.obsId); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.obsId);
    }

    /* vendor header options - implemented as getter method */
    field = (*env)->GetFieldID(env, k_request_in, "vendorHeaderOptionCount", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for vendorHeaderOptionCount fld\n");
    } else {
	/* printf("c nbf header options: %d\n", crequest_in->numRcvdVendorSpecificHeaderOptions); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->numRcvdVendorSpecificHeaderOptions);
    }

    /* message ID */
    field = (*env)->GetFieldID(env, k_request_in, "messageId", "I");
    if (field == NULL) { /* make sure we got the field */
	THROW_JNI_EXCEPTION("GetFieldID failed for messageId fld\n");
    } else {
	/* printf("message ID in c: %d\n", crequest_in->messageID); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->messageID);
    }

    /* payload - implemented as getter, using handle */
    field = (*env)->GetFieldID(env, k_request_in, "payloadHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for payloadHandle fld\n");
    } else {
	/* printf("OCPayload ptr (in c): %ld\n", (long)crequest_in->payload); */
	/* printf("OCPayload type: %ld\n", (long)((OCPayload*)crequest_in->payload)->type); */
	(*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->payload);
    }
    /* printf("populate_request_in EXIT\n"); */
}

/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_routine(OCEntityHandlerFlag flag,
				      OCEntityHandlerRequest * crequest_in, /* RequestIn */
				      void* callbackParam)
{
    OC_UNUSED(flag);
    printf("\nocf_resource_manager.c/service_routine ENTRY\n");
    /* printf("REQUEST URI: %s\n", ((OCResource*)(crequest_in->resource))->uri); */

    /* printf("request resource properties: 0x%X\n", */
    /* 	   ((OCResource*)(crequest_in->resource))->resourceProperties); */

    /* 1. set up jvm, env */
/* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
/* http://adamish.com/blog/archives/327 */
    JNIEnv * env;
    // double check it's all ok
    int getEnvStat = (*g_jvm)->GetEnv(g_jvm, (void **)&env, JNI_VERSION_1_8);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_jvm)->AttachCurrentThread(g_jvm, (void **) &env, NULL) != 0) {
	    /* THROW_JNI_EXCEPTION("AttachCurrentThread failed\n"); */
	    printf("ERROR %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    if (callbackParam == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): callbackParam is NULL\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* extract ref to service provider from callback param */
    jclass k_cbparam = (*env)->GetObjectClass(env, callbackParam);
    if (k_cbparam == NULL) {
	printf("ERROR %s %d (%s): GetObjectClass failed for callbackParam\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    jfieldID fid_rsp = (*env)->GetFieldID(env, k_cbparam,
					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
    if (fid_rsp == NULL) {
	printf("Failed to get serviceProvider fld id\n");
    }
    jobject service_provider = (*env)->GetObjectField(env, callbackParam, fid_rsp);
    if (service_provider == NULL) {
	printf("Failed to get ResourceServiceProvider object\n");
    }
    /* now get the service routine, for later */
    jclass k_rsp = (*env)->GetObjectClass(env, service_provider);
    if (k_rsp == NULL) {
	printf("Failed to find class for ResourceServiceProvider\n");
    }
    jmethodID mid_service = (*env)->GetMethodID(env, k_rsp,
					  "service",
					  "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
    if (mid_service == NULL) {
	printf("TEST Unable to get 'service' method id\n");
    }

    /* prepare args to service method: (ILorg/iochibity/RequestIn;Ljava/lang/Object;)I */

    /* create RequestIn object */
    jclass k_request_in = (*env)->FindClass(env, "Lorg/iochibity/RequestIn;");
    if (k_request_in == NULL) {
	printf("Failed to find RequestIn class\n");
    }

    jmethodID request_in_ctor = (*env)->GetMethodID(env, k_request_in, "<init>", "()V");
    if (request_in_ctor == NULL) {
	printf("Failed to find ctor method\n");
    }

    jobject jrequest_in = (*env)->NewObject(env, k_request_in, request_in_ctor);
    populate_request_in(env, jrequest_in, crequest_in);
    fflush(NULL);

    OCEntityHandlerResult op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env, service_provider, mid_service,
				      flag, jrequest_in, callbackParam);
    if (op_result != OC_STACK_OK) {
        printf("TEST call to service routine failed!\n");
        /* exit (EXIT_FAILURE); */
    } else {
        printf("TEST call to Java service routine succeeded!\n");
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    /* printf("Incoming request: %ld\n", (long)crequest_in); */
    /* printf("Incoming requestHandle: %ld\n", (long)crequest_in->requestHandle); */
    /* printf("Incoming request param: %ld\n", (long)callbackParam); */
    /* printf("Incoming request flag: %d\n", flag); */

    printf("ocf_resource_manager.c/service_routine EXIT\n");
    return op_result;
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    resourceCount
 * Signature: (B)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ResourceManager_resourceCount
(JNIEnv * env, jclass klass)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OCStackResult op_result;
    uint8_t count = 0;
    op_result = OCGetNumberOfResources(&count);
    return count;
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    RegisterDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_ResourceManager_registerDefaultServiceRoutine
(JNIEnv * env, jclass klass, jobject j_resource_service_provider, jobject j_callback_param)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_resource_service_provider);
    OC_UNUSED(j_callback_param);
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    RegisterResource
 * Signature: (Lorg/iochibity/Resource;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/iochibity/ResourceServiceProvider;Ljava/lang/Object;B)I
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceManager_registerResource
  (JNIEnv * env, jclass klass,
   jstring j_uri,
   jobjectArray j_tnames,
   jobjectArray j_ifnames,
   jobject j_resource_service_provider,
   jobject j_callback_param,
   jbyte   j_policies)
{
    OC_UNUSED(klass);
    printf("Java_org_iochibity_ResourceManager_registerResource ENTRY\n");

    jobject j_resource_type_name = (*env)->GetObjectArrayElement(env, j_tnames, 0);
    const char *c_resource_type_name = "";
    if (j_resource_type_name == NULL) {
	j_resource_type_name = (*env)->NewStringUTF(env, c_resource_type_name);
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
    } else {
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
	if (c_resource_type_name == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    jobject j_resource_if_name = (*env)->GetObjectArrayElement(env, j_ifnames, 0);
    const char *c_resource_if_name = "";

    if (j_resource_if_name == NULL) {
	j_resource_if_name = (*env)->NewStringUTF(env, c_resource_if_name);
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
    } else {
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
	if (c_resource_if_name == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }

    const char *c_uri = "";
    if (j_uri == NULL) {
	j_uri = (*env)->NewStringUTF(env, c_uri);
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
    } else {
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
	if (c_uri == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }

    /* prep callback */
    (*env)->GetJavaVM(env, &g_jvm);
    /* bool returnValue = true; */

    jobject g_resource_service_provider = NULL;
    jobject g_callback_param = NULL;
    if (j_resource_service_provider == NULL) {
	THROW_STACK_EXCEPTION(OC_STACK_INVALID_PARAM, "ResourceServiceProvider must not be null");
    } else {
	// convert local to global references to be passed
	g_resource_service_provider = (*env)->NewGlobalRef(env, j_resource_service_provider);
	jclass k_g_rsp = (*env)->GetObjectClass(env, g_resource_service_provider);
	if (k_g_rsp == NULL) {
	    THROW_JNI_EXCEPTION("GetObjectClass for ResourceServiceProvider impl");
	}
	jmethodID service_mid = (*env)->GetMethodID(env, k_g_rsp,
						    "service",
						    "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
	if (service_mid == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID: service routine");
	}

	/* now malloc a callbackParam struct and pack obj and method id into it */
	g_callback_param = (*env)->NewGlobalRef(env, j_callback_param);
	jclass k_cbparam = (*env)->GetObjectClass(env, g_callback_param);
	if (k_cbparam == NULL) {
	    THROW_JNI_EXCEPTION("GetObjectClass failed for CallbackParam\n");
	} else {
	    jfieldID fid_rsp = (*env)->GetFieldID(env, k_cbparam,
						  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
	    if (fid_rsp == NULL) {
		THROW_JNI_EXCEPTION("GetFieldID failed for serviceProvider");
	    } else {
		(*env)->SetObjectField(env, g_callback_param, fid_rsp, g_resource_service_provider);
	    }
	}
    }

    OCResourceHandle c_resource_handle;
    OCStackResult op_result;
    op_result = OCCreateResource( &c_resource_handle,    /* **Resource */
				  c_resource_type_name,  /* const char *resourceTypeName, */
				  c_resource_if_name,    /* const char *resourceInterfaceName, */
				  c_uri,                 /* const char *uri, */
				  service_routine,       /* OCEntityHandler entityHandler, */
				  g_callback_param,
				  (uint8_t) j_policies);  /* uint8_t resourceProperties */

    if (op_result != OC_STACK_OK) THROW_STACK_EXCEPTION(op_result, "OCCreateResource failed");

    /* if multiple types/ifs supplied, add them here */

    /* Populate Resource */
    /* printf("Resource handle: %ld\n", (long)c_resource_handle); */
    /* printf("Resource uri: %s\n", ((OCResource*)c_resource_handle)->uri); */

    /* create a Resource object to return */
    jclass k_resource = (*env)->FindClass(env, "Lorg/iochibity/Resource;");
    if (k_resource == NULL) {
	THROW_JNI_EXCEPTION("FindClass failed for org/iochibigy/Resource\n");
    }
    jmethodID resource_ctor = (*env)->GetMethodID(env, k_resource, "<init>", "()V");
    if (resource_ctor == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for Resource ctor\n");
    }
    jobject j_resource = (*env)->NewObject(env, k_resource, resource_ctor);

    /* insert handle to OCResource */
    if (k_resource != NULL) { /* cannot be null in this case */
        jfieldID fid_handle = (*env)->GetFieldID(env, k_resource, "handle", "J");
        if (fid_handle != NULL) {
	    (*env)->SetLongField(env, j_resource, fid_handle, (jlong) (OCResource*)c_resource_handle);
	}
        jfieldID urifield = (*env)->GetFieldID(env, k_resource, "uri", "Ljava/lang/String;");
        if (urifield != NULL) {
            jstring jString = (*env)->NewStringUTF(env, ((OCResource*)c_resource_handle)->uri);
            if (jString != NULL) {
                (*env)->SetObjectField(env, j_resource, urifield, jString);
            }
	}

        jfieldID fid_policies = (*env)->GetFieldID(env, k_resource, "policies", "I");
        if (fid_policies != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_policies,
				(jint)((OCResource*)c_resource_handle)->resourceProperties);
	} else {
	    THROW_JNI_EXCEPTION("SetIntField failed for fid_policies\n");
	}

	/* printf("resource seq nbr: %d\n", (jint) ((OCResource*)c_resource_handle)->sequenceNum); */
        jfieldID fid_sn = (*env)->GetFieldID(env, k_resource, "sn", "I");
        if (fid_sn != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_sn,
				(jint) ((OCResource*)c_resource_handle)->sequenceNum);
	}

	/* instance id ∪nion */
	/* printf("instance ORD:  %d\n", ((OCResource*)c_resource_handle)->ins); */
	/* printf("instance str:  '%s'\n", ((OCResource*)c_resource_handle)->uniqueStr); */
	/* printf("instance uuid ln: %d\n", ((OCResource*)c_resource_handle)->uniqueUUID.id_length); */
	/* printf("instance uuid: %s\n", ((OCResource*)c_resource_handle)->uniqueUUID.id); */

	if ( ((OCResource*)c_resource_handle)->ins > 0) {
	    printf("instance ORD:  %d\n", ((OCResource*)c_resource_handle)->ins);
	} else if ( ((OCResource*)c_resource_handle)->uniqueStr != NULL) {
	    printf("instance str:  '%s'\n", ((OCResource*)c_resource_handle)->uniqueStr);
	} else if ( ((OCResource*)c_resource_handle)->uniqueUUID.id_length > 0) {
	    printf("instance uuid: %s\n", ((OCResource*)c_resource_handle)->uniqueUUID.id);
	} else {
	    /* printf("NO INSTANCE ID\n"); */
	}
    }

    (*env)->ReleaseStringUTFChars(env, j_resource_type_name, c_resource_type_name);
    (*env)->ReleaseStringUTFChars(env, j_resource_if_name, c_resource_if_name);
    (*env)->ReleaseStringUTFChars(env, j_uri, c_uri);

    printf("org_iochibity_ResourceManager/registerResource EXIT\n");
    return j_resource;
}
