package org.iochibity.test.client;

import org.iochibity.CoServiceProvider;
import org.iochibity.constants.Method;
import org.iochibity.ObservationIn;
import org.iochibity.IObservationRecord;
import org.iochibity.ObservationRecord;
import org.iochibity.ObservationList;
import org.iochibity.ServiceManager;

import org.iochibity.constants.OCStackResult;

import org.iochibity.test.Logger;

import java.util.List;

public class DiscoveryCoSP
    extends  CoServiceProvider
    // implements ICoServiceProvider
{
    private int cbdata = 99;

    public DiscoveryCoSP() {
	super();
	setMethod(Method.GET);
    }
    public DiscoveryCoSP(String uri) {
	super();
	setMethod(Method.GET);
	setUriPath(uri);
    }

    public int observeBehavior(ObservationIn observationIn)
    {
	System.out.println("JAVA: DiscoveryCoSP.serviceResponse ENTRY");
	System.out.println("JAVA: cbdata: " + cbdata);
	Logger.logObservationIn(observationIn);

	// first update _this_ with incoming data
	this.setDestination(observationIn.getRemoteDeviceAddress());
	// etc. connType, etc.

	// then iterate over Observation payloads
	if (observationIn.result == OCStackResult.OK) {
	    ObservationList<ObservationRecord> observationRecords = observationIn.getObservationRecords();
	    for (ObservationRecord observationRecord
		     : (ObservationList<ObservationRecord>) observationRecords) {
		System.out.println("\tOBSERVED: " + observationRecord.getUriPath());
		List<IObservationRecord> kids = observationRecord.getChildren();
		if (kids != null) {
		    for (IObservationRecord childObservationRecord : kids) {
			System.out.println("\t->OBSERVED: " + childObservationRecord.getUriPath());

			GenericCoSP cosp = new GenericCoSP(observationIn, (ObservationRecord)childObservationRecord);
			ServiceManager.registerCoServiceProvider(cosp);

			if (childObservationRecord.getUriPath().equals("/a/temperature")) {
			    System.out.println("LOG: found temperature resource");
			    // gRemoteResourceAddress = observationIn.getRemoteDeviceAddress();
			    // gRemoteResourceAddress.port
			    // 	= ((Integer)childObservation.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gRemoteResourceAddress);
			}

			if (childObservationRecord.getUriPath().equals("/a/led")) {
			    System.out.println("LOG: found LED resource");
			    // gLEDAddress = observationIn.getRemoteDeviceAddress();
			    // gLEDAddress.port
			    // 	= ((Integer)childObservationRecord.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gLEDAddress);
			}

			if (childObservationRecord.getUriPath().equals("/a/whatsit")) {
			    System.out.println("LOG: found whatsit resource");
			    // gWhatsitAddress = observationIn.getRemoteDeviceAddress();
			    // gWhatsitAddress.port
			    // 	= ((Integer)childObservation.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gWhatsitAddress);
			}
		    }
		}
	    }
	}

	return 0;
    }

}

