package poop;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;

import javax.imageio.ImageIO;
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
import poop.Rectangle.Coordinate;

abstract public class Formatter
{

	private static ArrayList<Pair<String, Formatter>> Formats;

	static
	{
		Formats = new ArrayList<Pair<String, Formatter>>();
		Pair<String, Formatter> tmp = new Pair<String, Formatter>(".bmp", new BMP());
		Formats.add(tmp);
		tmp = new Pair<String, Formatter>(".pam", new PAM());
		Formats.add(tmp);
		tmp = new Pair<String, Formatter>(".mn", new MyFormat());
		Formats.add(tmp);
	}

	public static Formatter Factory(String Extension)
	{
		for (Pair<String, Formatter> tmp : Formats)
			if (tmp.getKey().equals(Extension))
				return tmp.getValue();
		return null;
	}

	public static void Reverse(ArrayList<Pixel> Pixels, int width, int height)
	{
		for (int i = 0, j = Pixels.size() - 1; i < j; i++, j--)
		{
			Pixel tmp = Pixels.get(i);
			Pixels.set(i, Pixels.get(j));
			Pixels.set(j, tmp);
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0, z = width - 1; j < z; j++, z--)
			{
				Pixel tmp = Pixels.get(j + width * i);
				Pixels.set(j + width * i, Pixels.get(z + width * i));
				Pixels.set(z + width * i, tmp);
			}
		}
	}

	protected void Check(Layer tmpLayer, int Width, int Height)
	{
		boolean flag1 = true;
		boolean flag2 = true;
		if (Width >= Layer.Width)
		{
			Layer.Width = Width;
			flag1 = false;
		}
		if (Height >= Layer.Height)
		{
			Layer.Height = Height;
			flag2 = false;
		}
		if (flag1 || flag2)
			tmpLayer.Resize(Width);
	}

	abstract public Layer Read(String Location) throws Exception;

	abstract public void Save(String Location, Layer SaveLayer) throws Exception;
}

class BMP extends Formatter
{

	@Override
	public Layer Read(String Location) throws Exception
	{
		if (!Location.endsWith(".bmp"))
			throw new Exception();
		BufferedImage img = null;
		img = ImageIO.read(new File(Location));
		int Width = img.getWidth();
		int Height = img.getHeight();
		Layer tmpLayer = null;
		ArrayList<Pixel> pixels = new ArrayList<Pixel>();
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
			{
				int tmpPixel = img.getRGB(j, i);
				int B = tmpPixel & 0x000000ff;
				tmpPixel = tmpPixel >> 8;
				int G = tmpPixel & 0x000000ff;
				tmpPixel = tmpPixel >> 8;
				int R = tmpPixel & 0x000000ff;
				tmpPixel = tmpPixel >> 8;
				int A = tmpPixel & 0x000000ff;
				pixels.add(new Pixel((byte) R, (byte) G, (byte) B, (byte) A));
			}
		Reverse(pixels, Width, Height);
		tmpLayer = new Layer(pixels);
		Check(tmpLayer, Width, Height);
		return tmpLayer;
	}

	@Override
	public void Save(String Location, Layer SaveLayer) throws IOException
	{
		String Loc;
		if (!Location.endsWith(".bmp"))
			Loc = Location + ".bmp";
		else
			Loc = Location;
		FileOutputStream FileOut = new FileOutputStream(new File(Loc));
		DataOutputStream DataOut = new DataOutputStream(FileOut);
		String BM = "BM";
		int SizeData = Layer.Width * Layer.Height * 4;
		int Size = SizeData + 122;
		int Offset = 122;
		int HeaderSize = 108;
		int BitColor = 0x200001;
		int BiBitFields = 3;
		int PrintResolution = 2835;
		int MaskR = 0xff000000;
		int MaskG = 0x00ff0000;
		int MaskB = 0x0000ff00;
		int MaskA = 0x000000ff;
		int LittleEndian = 0x57696e20;
		DataOut.write(BM.getBytes());
		DataOut.writeInt(Integer.reverseBytes(Size));
		DataOut.writeInt(0);
		DataOut.writeInt(Integer.reverseBytes(Offset));
		DataOut.writeInt(Integer.reverseBytes(HeaderSize));
		DataOut.writeInt(Integer.reverseBytes(Layer.Width));
		DataOut.writeInt(Integer.reverseBytes(Layer.Height));
		DataOut.writeInt(Integer.reverseBytes(BitColor));
		DataOut.writeInt(Integer.reverseBytes(BiBitFields));
		DataOut.writeInt(Integer.reverseBytes(SizeData));
		DataOut.writeInt(Integer.reverseBytes(PrintResolution));
		DataOut.writeInt(Integer.reverseBytes(PrintResolution));
		for (int i = 0; i < 2; i++)
			DataOut.writeInt(0);
		DataOut.writeInt(MaskR);
		DataOut.writeInt(MaskG);
		DataOut.writeInt(MaskB);
		DataOut.writeInt(MaskA);
		DataOut.writeInt(Integer.reverseBytes(LittleEndian));
		for (int i = 0; i < 12; i++)
			DataOut.writeInt(0);
		for (Pixel tmp : SaveLayer.getPixels())
		{
			DataOut.write(tmp.getR());
			DataOut.write(tmp.getG());
			DataOut.write(tmp.getB());
			DataOut.write(tmp.getA());
		}
		DataOut.flush();
		FileOut.close();
	}

}

class PAM extends Formatter
{
	@Override
	public Layer Read(String Location) throws Exception
	{
		if (!Location.endsWith(".pam"))
			throw new Exception();
		File FileIn = new File(Location);
		BufferedReader Reader = null;
		Reader = new BufferedReader(new FileReader(FileIn));
		Layer tmpLayer = null;
		Reader.readLine();
		String tmp;
		int[] WHD = new int[3];// Width Height Depth
		for (int i = 0; i < WHD.length; i++)
		{
			tmp = Reader.readLine();
			tmp = tmp.substring(tmp.indexOf(' ') + 1);
			WHD[i] = Integer.parseInt(tmp);
		}
		for (int i = 0; i < 3; i++)
			Reader.readLine();
		ArrayList<Pixel> Pixels = new ArrayList<Pixel>();
		for (int i = 0; i < WHD[1]; i++)
			for (int j = 0; j < WHD[0]; j++)
			{
				byte R = (byte) Reader.read();
				byte G = (byte) Reader.read();
				byte B = (byte) Reader.read();
				if (WHD[2] > 3)
				{
					byte A = (byte) Reader.read();
					Pixels.add(new Pixel(R, G, B, A));
				}
				else
					Pixels.add(new Pixel(R, G, B, (byte) 0xff));
			}

		Reader.close();
		Reverse(Pixels, WHD[0], WHD[1]);
		tmpLayer = new Layer(Pixels);
		Check(tmpLayer, WHD[0], WHD[1]);
		return tmpLayer;
	}

	@Override
	public void Save(String Location, Layer SaveLayer) throws IOException
	{
		String Loc;
		if (!Location.endsWith(".pam"))
			Loc = Location + ".pam";
		else
			Loc = Location;
		FileOutputStream FileOut;
		FileOut = new FileOutputStream(new File(Loc));
		String P7 = "P7\n";
		String width = "WIDTH " + Layer.Width + "\n";
		String height = "HEIGHT " + Layer.Height + "\n";
		String depth = "DEPTH 4\n";
		String maxValue = "MAXVAL 255\n";
		String type = "TUPLTYPE RGB_ALPHA\n";
		String end = "ENDHDR\n";
		FileOut.write(P7.getBytes());
		FileOut.write(width.getBytes());
		FileOut.write(height.getBytes());
		FileOut.write(depth.getBytes());
		FileOut.write(maxValue.getBytes());
		FileOut.write(type.getBytes());
		FileOut.write(end.getBytes());
		Reverse(SaveLayer.getPixels(), Layer.Width, Layer.Height);
		for (Pixel tmp : SaveLayer.getPixels())
		{
			FileOut.write(tmp.getR());
			FileOut.write(tmp.getG());
			FileOut.write(tmp.getB());
			FileOut.write(tmp.getA());
		}
		FileOut.flush();
		FileOut.close();
		Reverse(SaveLayer.getPixels(), Layer.Width, Layer.Height);
	}

}

class MyFormat extends Formatter
{

	@Override
	public Layer Read(String Location) throws Exception
	{
		if (!Location.endsWith(".mn"))
			throw new Exception();
		File file = new File(Location);
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document document = builder.parse(file);
		document.getDocumentElement().normalize();
		Layer ReturnLayer = null;

		Node root = document.getFirstChild();
		while (root.getNodeType() != Node.ELEMENT_NODE)
			root = root.getNextSibling();
		// Layers
		Node depth1 = root.getFirstChild();
		while (depth1.getNodeType() != Node.ELEMENT_NODE)
			depth1 = depth1.getNextSibling();
		// NumberOfLayers
		Node depth2 = depth1.getFirstChild();
		while (depth2.getNodeType() != Node.ELEMENT_NODE)
			depth2 = depth2.getNextSibling();
		int Number;
		Number = Integer.parseInt(depth2.getTextContent());

		Node depth3;
		for (int i = 0; i < Number; i++)
		{
			// Layer
			depth2 = depth2.getNextSibling();
			while (depth2.getNodeType() != Node.ELEMENT_NODE)
				depth2 = depth2.getNextSibling();
			depth3 = depth2.getFirstChild();
			while (depth3.getNodeType() != Node.ELEMENT_NODE)
				depth3 = depth3.getNextSibling();
			boolean Active = (Integer.parseInt(depth3.getTextContent()) == 1);
			depth3 = depth3.getNextSibling();
			while (depth3.getNodeType() != Node.ELEMENT_NODE)
				depth3 = depth3.getNextSibling();
			boolean Visibility = (Integer.parseInt(depth3.getTextContent()) == 1);
			depth3 = depth3.getNextSibling();
			while (depth3.getNodeType() != Node.ELEMENT_NODE)
				depth3 = depth3.getNextSibling();
			double Transparency = Double.parseDouble(depth3.getTextContent());
			ReturnLayer = Formatter.Factory(".bmp").Read(file.getParent() + "\\Layers\\Layer" + i + ".bmp");
			ReturnLayer.setActive(Active);
			ReturnLayer.setVisible(Visibility);
			ReturnLayer.setTransparency(Transparency);
			if (i != (Number - 1))
				Picture.GetInstance().getLayers().add(ReturnLayer);
		}

		// CompositeOperations
		CompositeOperation.GetCompositeOperations().clear();
		depth1 = depth1.getNextSibling();
		while (depth1.getNodeType() != Node.ELEMENT_NODE)
			depth1 = depth1.getNextSibling();
		Number = Integer.parseInt(depth1.getTextContent());
		for (int i = 0; i < Number; i++)
		{
			CompositeOperation compositeOperation = new CompositeOperation(file.getParent() + "\\CompositeOperation\\CompositeOperation" + i + ".fun");
			CompositeOperation.GetCompositeOperations().add(compositeOperation);
		}

		// Selections
		for (depth1 = depth1.getNextSibling(); depth1 != null; depth1 = depth1.getNextSibling())
		{
			if (depth1.getNodeType() != Node.ELEMENT_NODE)
				continue;
			depth2 = depth1.getFirstChild();
			while (depth2.getNodeType() != Node.ELEMENT_NODE)
				depth2 = depth2.getNextSibling();
			String Name = depth2.getTextContent();
			depth2 = depth2.getNextSibling();
			while (depth2.getNodeType() != Node.ELEMENT_NODE)
				depth2 = depth2.getNextSibling();
			boolean Active = (Integer.parseInt(depth2.getTextContent()) == 1);
			ArrayList<Rectangle> rectangles = new ArrayList<Rectangle>();
			for (depth2 = depth2.getNextSibling(); depth2 != null; depth2 = depth2.getNextSibling())
			{
				if (depth2.getNodeType() != Node.ELEMENT_NODE)
					continue;
				depth3 = depth2.getFirstChild();
				while (depth3.getNodeType() != Node.ELEMENT_NODE)
					depth3 = depth3.getNextSibling();
				int x = Integer.parseInt(depth3.getTextContent());
				depth3 = depth3.getNextSibling();
				while (depth3.getNodeType() != Node.ELEMENT_NODE)
					depth3 = depth3.getNextSibling();
				int y = Integer.parseInt(depth3.getTextContent());
				depth3 = depth3.getNextSibling();
				while (depth3.getNodeType() != Node.ELEMENT_NODE)
					depth3 = depth3.getNextSibling();
				int Width = Integer.parseInt(depth3.getTextContent());
				depth3 = depth3.getNextSibling();
				while (depth3.getNodeType() != Node.ELEMENT_NODE)
					depth3 = depth3.getNextSibling();
				int Height = Integer.parseInt(depth3.getTextContent());
				int x2 = x + Width;
				int y2 = y - Height;
				rectangles.add(new Rectangle(new Coordinate(x, y), new Coordinate(x2, y2)));
			}
			Selection tmpSelection = new Selection(rectangles, Name);
			tmpSelection.setActive(Active);
			Picture.GetInstance().getSelections().add(tmpSelection);
		}
		return ReturnLayer;

	}

	@Override
	public void Save(String Location, Layer SaveLayer) throws Exception
	{
		String Loc;
		if (!Location.endsWith(".mn"))
			Loc = Location + ".mn";
		else
			Loc = Location;

		Picture picture = Picture.GetInstance();
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document document = builder.newDocument();
		Element root = document.createElement("Project");
		document.appendChild(root);

		Element depth1 = document.createElement("Layers");
		root.appendChild(depth1);
		Element depth2 = document.createElement("NumberOfLayers");
		depth2.setTextContent(Integer.toString(picture.getLayers().size()));
		depth1.appendChild(depth2);
		Element depth3;
		for (Layer tmp : picture.getLayers())
		{
			depth2 = document.createElement("Layer");
			depth3 = document.createElement("Active");
			depth3.setTextContent(tmp.isActive() ? "1" : "0");
			depth2.appendChild(depth3);
			depth3 = document.createElement("Visibility");
			depth3.setTextContent(tmp.isVisible() ? "1" : "0");
			depth2.appendChild(depth3);
			depth3 = document.createElement("Transparency");
			depth3.setTextContent(Double.toString(tmp.getTransparency()));
			depth2.appendChild(depth3);
			depth1.appendChild(depth2);
		}

		depth1 = document.createElement("CompositeOperations");
		depth1.setTextContent(Integer.toString(CompositeOperation.GetCompositeOperations().size()));
		root.appendChild(depth1);

		for (Selection tmp : picture.getSelections())
		{
			depth1 = document.createElement("Selection");
			depth2 = document.createElement("Name");
			depth2.setTextContent(tmp.getName());
			depth1.appendChild(depth2);
			depth2 = document.createElement("Active");
			depth2.setTextContent(tmp.isActive() ? "1" : "0");
			depth1.appendChild(depth2);
			for (Rectangle tmpR : tmp.getRectangles())
			{
				depth2 = document.createElement("Rectangle");
				depth3 = document.createElement("x");
				depth3.setTextContent(Integer.toString(tmpR.getTopLeft().getX()));
				depth2.appendChild(depth3);
				depth3 = document.createElement("y");
				depth3.setTextContent(Integer.toString(tmpR.getTopLeft().getY()));
				depth2.appendChild(depth3);
				depth3 = document.createElement("Width");
				depth3.setTextContent(Integer.toString(tmpR.getWidth()));
				depth2.appendChild(depth3);
				depth3 = document.createElement("Height");
				depth3.setTextContent(Integer.toString(tmpR.getHeight()));
				depth2.appendChild(depth3);
				depth1.appendChild(depth2);
			}
			root.appendChild(depth1);
		}

		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		DOMSource source = new DOMSource(document);
		StreamResult result = new StreamResult(new File(Loc));
		transformer.transform(source, result);

		Path path = Paths.get(Loc);
		path = Paths.get(path.getParent() + "\\Layers");
		File file = new File(path.toString());
		file.mkdir();
		int i = 0;
		Formatter tmpFormatter = Formatter.Factory(".bmp");
		for (Layer tmp : picture.getLayers())
			tmpFormatter.Save(file.getPath() + "\\Layer" + i++, tmp);
		path = Paths.get(path.getParent() + "\\CompositeOperation");
		file = new File(path.toString());
		file.mkdir();
		i = 0;
		for (CompositeOperation tmp : CompositeOperation.GetCompositeOperations())
			tmp.SaveXml(file.getPath() + "\\CompositeOperation" + i++);

	}

}