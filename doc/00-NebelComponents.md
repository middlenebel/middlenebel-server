
# NebelComponents

A NebelComponent is and scriptable object with the syntax:

```
    ComponentName : [name]
        attribute1 value1
        attributeN valueN
        ...
        [Sub-component (or Commands)]
    .
```

That is;

- **ComponentName**: ClassName of the component to create, begin with Uppercase followed of ':'
- **[name]**: Is an optional name (not all components has name) that identify this component instance.
- **attributeN**: The name of attributes of the component and his value.
- **valueN**: Values of the attributes, are string. If has special chars can be in the format:
    + 'value.with-special/characteres'
- **Commands** Are Coomponents that execute actions. Has the same syntax than a Component.
- **End point '.'** to finish the Component definition.

## Components (Commands)
They begin with an uppercase letter and can have only a keyword or a body too.

The body begins with ':' and ends with  '.'

```
Component

(or)

Component: [body] .
```

The first word before ':' is an optional "name" attribute:

```
Component: name
    [....]
. 
```

## Sub-components
Some components can have other sub-components in his body.

For example **K8S** can include **Deployment** and **Service** subcomponents.

**Deployment** can include **Env** subcomponent, etc.

- **CreateNamespaces**: name[,...] .

As all commands start with ':' and finish  with '.'.

Define the attribute 'namespace' to the last namespace name created.

## Attributes
You can pass attributes values to the obects. For example:

```
    namespace 'a-namespace-name'
```
