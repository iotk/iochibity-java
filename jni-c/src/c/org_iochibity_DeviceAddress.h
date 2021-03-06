/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_iochibity_DeviceAddress */

#ifndef _Included_org_iochibity_DeviceAddress
#define _Included_org_iochibity_DeviceAddress
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkProtocol
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_networkProtocol
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkFlags
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_networkFlags
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    transportIsSecure
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_transportIsSecure
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    isIPv4
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isIPv4
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    isIPv6
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isIPv6
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    isMulticast
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isMulticast
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkScope
 * Signature: ()B
 */
JNIEXPORT jbyte JNICALL Java_org_iochibity_DeviceAddress_networkScope
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    port
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_port
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    ipAddress
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_iochibity_DeviceAddress_ipAddress
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    ifindex
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_ifindex
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    routeData
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_iochibity_DeviceAddress_routeData
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
