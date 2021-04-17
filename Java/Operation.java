package poop;

import java.io.File;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import javafx.util.Pair;

public abstract class Operation implements Cloneable
{

	private static ArrayList<Pair<String, Operation>> Operations;

	static
	{
		Operations = new ArrayList<Pair<String, Operation>>();
		Operation Op;
		Op = new OperationInverse();
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationGrey();
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationBlackAndWhite();
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationAbs();
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationBlur();
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationLog(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationAdd(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationSub(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationInverseSub(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationMul(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationDiv(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationInverseDiv(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationPow(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationMin(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationMax(1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		Op = new OperationFill("", 1, 1, 1);
		Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		try
		{
			Op = new CompositeOperation("");
			Operations.add(new Pair<String, Operation>(Op.GetXml(), Op));
		}
		catch (Exception e)
		{
			System.out.println("Ovde je teorecki nemoguce da se dodje");
			System.exit(-1);
		}
	}

	public static ArrayList<Pair<String, Operation>> GetOperations()
	{
		return Operations;
	}

	public static Operation Factory(String OperationName)
	{
		for (Pair<String, Operation> x : Operations)
			if (x.getKey().equals(OperationName))
				return (Operation) x.getValue().clone();
		return null;
	}

	public abstract String GetXml();

	public void Set(Pair<ArrayList<Double>, String> Values) throws Exception
	{
	}

	public Pair<ArrayList<Double>, String> Get()
	{
		return null;
	}

	public int NumOfPar()
	{
		return 0;
	}

	public int NumOfStringPar()
	{
		return 0;
	}

	@Override
	protected Object clone()
	{
		return null;
	}
}

class OperationInverse extends Operation implements Cloneable
{

	@Override
	public String GetXml()
	{
		return "Inverse";
	}

	@Override
	protected Object clone()
	{
		OperationInverse tmp = new OperationInverse();
		return tmp;
	}

}

class OperationGrey extends Operation implements Cloneable
{

	@Override
	public String GetXml()
	{
		return "Grey";
	}

	@Override
	protected Object clone()
	{
		OperationGrey tmp = new OperationGrey();
		return tmp;
	}

}

class OperationBlackAndWhite extends Operation implements Cloneable
{

	@Override
	public String GetXml()
	{
		return "BlackAndWhite";
	}

	@Override
	protected Object clone()
	{
		OperationBlackAndWhite tmp = new OperationBlackAndWhite();
		return tmp;
	}

}

class OperationAbs extends Operation implements Cloneable
{

	@Override
	public String GetXml()
	{
		return "Abs";
	}

	@Override
	protected Object clone()
	{
		OperationAbs tmp = new OperationAbs();
		return tmp;
	}

}

class OperationBlur extends Operation implements Cloneable
{

	@Override
	public String GetXml()
	{
		return "Blur";
	}

	@Override
	protected Object clone()
	{
		OperationBlur tmp = new OperationBlur();
		return tmp;
	}

}

class OperationLog extends Operation implements Cloneable
{

	private double Base;

	public OperationLog(double Base)
	{
		super();
		this.Base = Base > 0 ? Base : 1;
	}

	@Override
	public String GetXml()
	{
		return "Log";
	}

	@Override
	protected Object clone()
	{
		OperationLog tmp = new OperationLog(Base);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Base = Values.getKey().get(0) > 0 ? Values.getKey().get(0) : 1;
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Base);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}

}

class OperationAdd extends Operation implements Cloneable
{

	private double Sum;

	public OperationAdd(double Sum)
	{
		super();
		this.Sum = Sum;
	}

	@Override
	public String GetXml()
	{
		return "Add";
	}

	@Override
	protected Object clone()
	{
		OperationAdd tmp = new OperationAdd(Sum);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Sum = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Sum);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationSub extends Operation implements Cloneable
{

	private double Sub;

	public OperationSub(double Sub)
	{
		super();
		this.Sub = Sub;
	}

	@Override
	public String GetXml()
	{
		return "Sub";
	}

	@Override
	protected Object clone()
	{
		OperationSub tmp = new OperationSub(Sub);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Sub = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Sub);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationInverseSub extends Operation implements Cloneable
{

	private double Sub;

	public OperationInverseSub(double Sub)
	{
		super();
		this.Sub = Sub;
	}

	@Override
	public String GetXml()
	{
		return "InverseSub";
	}

	@Override
	protected Object clone()
	{
		OperationInverseSub tmp = new OperationInverseSub(Sub);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Sub = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Sub);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationMul extends Operation implements Cloneable
{

	private double Mul;

	public OperationMul(double Mul)
	{
		super();
		this.Mul = Mul;
	}

	@Override
	public String GetXml()
	{
		return "Mul";
	}

	@Override
	protected Object clone()
	{
		OperationMul tmp = new OperationMul(Mul);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Mul = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Mul);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationDiv extends Operation implements Cloneable
{

	private double Div;

	public OperationDiv(double Div)
	{
		super();
		this.Div = Div != 0 ? Div : 1;
	}

	@Override
	public String GetXml()
	{
		return "Div";
	}

	@Override
	protected Object clone()
	{
		OperationDiv tmp = new OperationDiv(Div);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Div = Values.getKey().get(0) != 0 ? Values.getKey().get(0) : 1;
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Div);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationInverseDiv extends Operation implements Cloneable
{

	private double Div;

	public OperationInverseDiv(double Div)
	{
		super();
		this.Div = Div;
	}

	@Override
	public String GetXml()
	{
		return "InverseDiv";
	}

	@Override
	protected Object clone()
	{
		OperationInverseDiv tmp = new OperationInverseDiv(Div);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Div = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Div);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationPow extends Operation implements Cloneable
{

	private double Pow;

	public OperationPow(double Pow)
	{
		super();
		this.Pow = Pow;
	}

	@Override
	public String GetXml()
	{
		return "Pow";
	}

	@Override
	protected Object clone()
	{
		OperationPow tmp = new OperationPow(Pow);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Pow = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Pow);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationMin extends Operation implements Cloneable
{

	private double Min;

	public OperationMin(double Min)
	{
		super();
		this.Min = Min;
	}

	@Override
	public String GetXml()
	{
		return "Min";
	}

	@Override
	protected Object clone()
	{
		OperationMin tmp = new OperationMin(Min);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Min = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Min);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationMax extends Operation implements Cloneable
{

	private double Max;

	public OperationMax(double Max)
	{
		super();
		this.Max = Max;
	}

	@Override
	public String GetXml()
	{
		return "Max";
	}

	@Override
	protected Object clone()
	{
		OperationMax tmp = new OperationMax(Max);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		Max = Values.getKey().get(0);
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(Max);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, "");
	}

	@Override
	public int NumOfPar()
	{
		return 1;
	}
}

class OperationFill extends Operation implements Cloneable
{

	private String Name;
	private double R, G, B;

	public OperationFill(String name, double r, double g, double b)
	{
		super();
		this.Name = name;
		R = r;
		G = g;
		B = b;
	}

	@Override
	public String GetXml()
	{
		return "Fill";
	}

	@Override
	protected Object clone()
	{
		OperationFill tmp = new OperationFill(Name, R, G, B);
		return tmp;
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values)
	{
		R = Values.getKey().get(0);
		G = Values.getKey().get(1);
		B = Values.getKey().get(2);
		Name = Values.getValue();
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		ArrayList<Double> tmpArrayList = new ArrayList<Double>();
		tmpArrayList.add(R);
		tmpArrayList.add(G);
		tmpArrayList.add(B);
		return new Pair<ArrayList<Double>, String>(tmpArrayList, Name);
	}

	@Override
	public int NumOfPar()
	{
		return 3;
	}

	@Override
	public int NumOfStringPar()
	{
		return 1;
	}

}

class CompositeOperation extends Operation implements Cloneable
{

	private String Location;
	private ArrayList<Operation> Operations;
	private static int count = 0;
	private static ArrayList<String> Locations = new ArrayList<String>();
	private static ArrayList<CompositeOperation> compositeOperations = new ArrayList<CompositeOperation>();

	public static ArrayList<CompositeOperation> GetCompositeOperations()
	{
		return compositeOperations;
	}

	public CompositeOperation(String location) throws Exception
	{
		super();
		Set(new Pair<ArrayList<Double>, String>(null, location));
	}

	public CompositeOperation(ArrayList<Operation> operations)
	{
		super();
		Operations = operations;
		Location = "";
	}

	@Override
	public void Set(Pair<ArrayList<Double>, String> Values) throws Exception
	{
		Location = Values.getValue();
		if (!Location.equals(""))
			LoadXml();
	}

	@Override
	public Pair<ArrayList<Double>, String> Get()
	{
		return new Pair<ArrayList<Double>, String>(null, Location);
	}

	@Override
	public String GetXml()
	{
		return "Composite";
	}

	@Override
	public int NumOfStringPar()
	{
		return 1;
	}

	private void LoadXml() throws Exception
	{
		if (!Location.endsWith(".fun"))
		{
			count = 0;
			Locations.clear();
			throw new Exception();
		}
		for (String s : Locations)
			if (Location.equals(s))
				return;
		Operations = new ArrayList<Operation>();
		count++;
		File file = new File(Location);
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document document = builder.parse(file);
		document.getDocumentElement().normalize();

		Node root = document.getFirstChild();
		while (root.getNodeType() != Node.ELEMENT_NODE)
			root = root.getNextSibling();
		for (Node operatioNode = root.getFirstChild(); operatioNode != null; operatioNode = operatioNode.getNextSibling())
		{
			if (operatioNode.getNodeType() != Node.ELEMENT_NODE)
				continue;
			Node nameNode = operatioNode.getFirstChild();
			while (nameNode.getNodeType() != Node.ELEMENT_NODE)
				nameNode = nameNode.getNextSibling();
			Operation op = Operation.Factory(nameNode.getTextContent());
			ArrayList<Double> tmpArrayList = new ArrayList<Double>();
			String nameString = "";
			for (Node valueNode = nameNode.getNextSibling(); valueNode != null; valueNode = valueNode.getNextSibling())
			{
				if (valueNode.getNodeType() != Node.ELEMENT_NODE)
					continue;
				if (valueNode.getNodeName().equals("StrVar"))
					nameString = valueNode.getTextContent();
				else
					tmpArrayList.add(Double.parseDouble(valueNode.getTextContent()));
			}
			op.Set(new Pair<ArrayList<Double>, String>(tmpArrayList, nameString));
			Operations.add(op);
		}
		count--;
		if (count == 0)
			Locations.clear();
	}

	public void SaveXml(String Location) throws Exception
	{
		if (Operations == null)
			throw new Exception();
		String Loc;
		if (!Location.endsWith(".fun"))
			Loc = Location + ".fun";
		else
			Loc = Location;

		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document document = builder.newDocument();
		Element root = document.createElement("Function");
		document.appendChild(root);

		for (Operation tmpOperation : Operations)
		{
			Element operationElement = document.createElement("Operation");
			Element tmpElement = document.createElement("OperationName");
			tmpElement.setTextContent(tmpOperation.GetXml());
			operationElement.appendChild(tmpElement);
			Pair<ArrayList<Double>, String> Values = tmpOperation.Get();
			if (Values != null)
			{
				if (!Values.getValue().equals(""))
				{
					tmpElement = document.createElement("StrVar");
					tmpElement.setTextContent(Values.getValue());
					operationElement.appendChild(tmpElement);
				}
				if (Values.getKey() != null)
				{
					for (Double tmpDoubles : Values.getKey())
					{
						tmpElement = document.createElement("Var");
						tmpElement.setTextContent(tmpDoubles.toString());
						operationElement.appendChild(tmpElement);
					}
				}
			}
			root.appendChild(operationElement);
		}

		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		DOMSource source = new DOMSource(document);
		StreamResult result = new StreamResult(new File(Loc));
		transformer.transform(source, result);
	}

	@Override
	protected Object clone()
	{
		CompositeOperation tmpCompositeOperation = new CompositeOperation(Operations);
		return tmpCompositeOperation;
	}

	public ArrayList<Operation> getOperations()
	{
		return Operations;
	}

}