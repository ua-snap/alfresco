using System;
using System.Collections;
using System.Reflection;
using System.Reflection.Emit;

/// <summary>
/// Thanks to John Wood on dotnetjunkies.com for this helpful class.
/// Author: John Wood, Thoughts on Enterprise Development
/// Wednesday, August 31, 2005
/// http://dotnetjunkies.com/WebLog/johnwood/archive/2005/08/31/132267.aspx
/// </summary>
public class SafeInvokeHelper
{
    static readonly ModuleBuilder builder;
    static readonly AssemblyBuilder myAsmBuilder;
    static readonly Hashtable methodLookup;

    static SafeInvokeHelper()
    {
        AssemblyName name = new AssemblyName();
        name.Name = "temp";
        myAsmBuilder = AppDomain.CurrentDomain.DefineDynamicAssembly(name, AssemblyBuilderAccess.Run);
        builder = myAsmBuilder.DefineDynamicModule("TempModule");
        methodLookup = new Hashtable();
    }

    /// <summary>
    /// Executes a control's function within the thread that owns the control.  
    /// See http://dotnetjunkies.com/WebLog/johnwood/archive/2005/08/31/132267.aspx.
    /// </summary>
    /// <param name="obj">The control that needs to execute a function.</param>
    /// <param name="methodName">The name of the control's function.</param>
    /// <param name="paramValues">The parameters to pass to the control's function.</param>
    /// <returns>The return value from the control's function.</returns>
    public static object Invoke(System.Windows.Forms.Control obj, string methodName, params object[] paramValues)
    {
        Delegate del = null;
        string key = obj.GetType().Name + "." + methodName;
        Type tp;
        lock (methodLookup) {
        if (methodLookup.Contains(key)) 
            tp = (Type)methodLookup[key];
        else
        {
            Type[] paramList = new Type[obj.GetType().GetMethod(methodName).GetParameters().Length];
            int n = 0;
            foreach (ParameterInfo pi in obj.GetType().GetMethod(methodName).GetParameters()) paramList[n++] = pi.ParameterType;
            TypeBuilder typeB = builder.DefineType("Del_" +  obj.GetType().Name + "_" + methodName, TypeAttributes.Class | TypeAttributes.AutoLayout | TypeAttributes.Public |  TypeAttributes.Sealed, typeof(MulticastDelegate), PackingSize.Unspecified);
            ConstructorBuilder conB = typeB.DefineConstructor(MethodAttributes.HideBySig | MethodAttributes.SpecialName |            MethodAttributes.RTSpecialName, CallingConventions.Standard, new Type[] { typeof(object), typeof(IntPtr) });
            conB.SetImplementationFlags(MethodImplAttributes.Runtime);
            MethodBuilder mb = typeB.DefineMethod( "Invoke", MethodAttributes.Public | MethodAttributes.Virtual | MethodAttributes.HideBySig, obj.GetType().GetMethod(methodName).ReturnType, paramList );
            mb.SetImplementationFlags( MethodImplAttributes.Runtime ); 
            tp = typeB.CreateType();
            methodLookup.Add(key, tp);
        }
        }

        del = MulticastDelegate.CreateDelegate(tp, obj, methodName);
        return obj.Invoke(del, paramValues);
    }
}