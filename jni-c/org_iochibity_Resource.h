/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_iochibity_Resource */

#ifndef _Included_org_iochibity_Resource
#define _Included_org_iochibity_Resource
#ifdef __cplusplus
extern "C" {
#endif
#undef org_iochibity_Resource_NOMETHOD
#define org_iochibity_Resource_NOMETHOD 0L
#undef org_iochibity_Resource_NONE
#define org_iochibity_Resource_NONE 0L
#undef org_iochibity_Resource_DISCOVERABLE
#define org_iochibity_Resource_DISCOVERABLE 1L
#undef org_iochibity_Resource_OBSERVABLE
#define org_iochibity_Resource_OBSERVABLE 2L
#undef org_iochibity_Resource_ACTIVE
#define org_iochibity_Resource_ACTIVE 4L
#undef org_iochibity_Resource_SLOW
#define org_iochibity_Resource_SLOW 8L
#undef org_iochibity_Resource_SECURE
#define org_iochibity_Resource_SECURE 16L
#undef org_iochibity_Resource_EXPLICIT_DISCOVERABLE
#define org_iochibity_Resource_EXPLICIT_DISCOVERABLE 32L
/*
 * Class:     org_iochibity_Resource
 * Method:    getTypes
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getTypes
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getInterfaces
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getInterfaces
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getProperties
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getProperties
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getChildren
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getChildren
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getServiceProvider
 * Signature: ()Lorg/iochibity/ResourceServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getServiceProvider
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getCallbackParam
 * Signature: ()Lorg/iochibity/CallbackParam;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getCallbackParam
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_Resource
 * Method:    getActionSet
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getActionSet
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
