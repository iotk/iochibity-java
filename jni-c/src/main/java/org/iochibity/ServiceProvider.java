package org.iochibity;

import java.util.List;
import java.util.LinkedList;

public abstract class ServiceProvider
    implements IServiceProvider
{
    private long                   _handle; // OCResource*
    public  long                   getHandle() { return _handle; }

    // OCResource fields
    private InstanceId             _id;
    public  InstanceId             getInstanceId() { return _id; }

    private String                 _uriPath;
    public  String                 getUriPath() { return _uriPath; }
    public  void                   setUriPath(String theUri) { _uriPath = theUri; }

    private List<String>           _types = new LinkedList<String>();
    public  List<String>           getTypes() { return _types; }
    public  boolean                addType(String theTypes) { return _types.add(theTypes); }

    private List<String>           _interfaces = new LinkedList<String>();
    public List<String>            getInterfaces() { return _interfaces; }
    public boolean                 addInterface(String iface) { return _interfaces.add(iface); }

    private PropertyMap            _properties;
    public  PropertyMap            getProperties() { return _properties; }
    public  Object                 putProperty(String key, Object val) { return _properties.put(key, val); }

    private List<IServiceProvider> _children;
    public  List<IServiceProvider> getChildren() { return _children; }

    // OCResource uint32_t sequenceNum -out of place, this is a Message serial
    // public int          getSerial();      // for observables, https://tools.ietf.org/html/rfc7641

    private List<ActionSet>        _actionSet;
    public  List<ActionSet>        getActionSet() { return _actionSet; }

    private int                    _policies = 0;
    public  int                    getPolicies() { return _policies; }
    public  void                   setPolicies(int newPolicies) { _policies = newPolicies; }

    // black boxing
    native public void             react(StimulusIn stimulusIn);           // called by user
    native public void             exhibitBehavior();                      // called by user
    // observeStimulus: implemented by user, called by stack
    abstract public int            observeStimulus(StimulusIn stimulusIn);
}