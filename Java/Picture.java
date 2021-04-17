package poop;

import java.util.ArrayList;

public class Picture
{
	private ArrayList<Selection> Selections = new ArrayList<Selection>();
	private ArrayList<Layer> Layers = new ArrayList<Layer>();
	private static Picture Pic = null;
	private boolean Saved = false;

	private Picture(int Width, int Height)
	{
		Layer.Width = Width;
		Layer.Height = Height;
		CreateNewLayer();
	}

	static Picture GetInstance()
	{
		return Pic;
	}

	static Picture GetInstance(int Width, int Height)
	{
		if (Pic == null)
			Pic = new Picture(Width, Height);
		return Pic;
	}

	static Picture GetInstance(Formatter formatter, String Location) throws Exception
	{
		if (Pic == null)
		{
			Pic = new Picture(0, 0);
			Pic.CreateNewLayer(formatter, Location);
			Pic.DeleteLayer(1);
		}
		return Pic;
	}

	static void DeleteInstance()
	{
		Pic = null;
	}

	public void CreateNewLayer()
	{
		ArrayList<Pixel> tmp = new ArrayList<Pixel>();
		for (int i = 0; i < Layer.Height * Layer.Width; i++)
			tmp.add(new Pixel((byte) 0, (byte) 0, (byte) 0, (byte) 255));
		Layers.add(new Layer(tmp));
		Saved = false;
	}

	public void CreateNewLayer(Formatter formatter, String Location) throws Exception
	{
		int OldWidth = Layer.Width;
		int OldHeight = Layer.Height;
		Layer tmpLayer = formatter.Read(Location);
		if (!Location.endsWith(".mn"))
			if (OldWidth != Layer.Width || OldHeight != Layer.Height)
				for (Layer x : Layers)
					x.Resize(OldWidth);
		Layers.add(tmpLayer);
		Saved = false;
	}

	public void DeleteLayer(int Position)
	{
		Layers.remove(Position - 1);
		Saved = false;
	}

	public void SetVisibleLayer(int Position, boolean Visible)
	{
		Layers.get(Position - 1).setVisible(Visible);
		Saved = false;
	}

	public void SetActiveLayer(int Position, boolean Active)
	{
		Layers.get(Position - 1).setActive(Active);
		Saved = false;
	}

	public void SetTransparencyLayer(int Position, double Transparency)
	{
		Layers.get(Position - 1).setTransparency(Transparency);
		Saved = false;
	}

	public void CreateNewSelection(String Name, ArrayList<Rectangle> Rectangles)
	{
		Selections.add(new Selection(Rectangles, Name));
	}

	public void SetSelectionActive(String Name, boolean Active)
	{
		for (Selection selection : Selections)
			if (selection.getName().equals(Name))
			{
				selection.setActive(Active);
				break;
			}
		Saved = false;
	}

	public void DeleteSelection(String Name)
	{
		for (Selection selection : Selections)
			if (selection.getName().equals(Name))
			{
				Selections.remove(selection);
				break;
			}
		Saved = false;
	}

	public void Save(Formatter formatter, String Location) throws Exception
	{
		formatter.Save(Location, CreatFinished(0));
		Saved = true;
	}

	public Layer CreatFinished(int index)
	{
		if (index >= Layers.size() || index < 0)
			return null;
		ArrayList<Pixel> tmp = new ArrayList<Pixel>();
		for (int i = 0; i < Layer.Width * Layer.Height; i++)
			tmp.add(new Pixel((byte) 0, (byte) 0, (byte) 0, (byte) 0));
		for (int i = 0; i < Layer.Width * Layer.Height; i++)
			for (int j = index; j < Layers.size(); j++)
				if (Layers.get(j).isVisible())
				{
					int A1, R1, G1, B1, A2, R2, G2, B2;
					A1 = tmp.get(i).getA() & 0xff;
					R1 = tmp.get(i).getR() & 0xff;
					G1 = tmp.get(i).getG() & 0xff;
					B1 = tmp.get(i).getB() & 0xff;
					A2 = Layers.get(j).getPixels().get(i).getA() & 0xff;
					R2 = Layers.get(j).getPixels().get(i).getR() & 0xff;
					G2 = Layers.get(j).getPixels().get(i).getG() & 0xff;
					B2 = Layers.get(j).getPixels().get(i).getB() & 0xff;
					double tmpA0 = A1 / 255.0;
					double tmpA1 = (Layers.get(j).getTransparency() / 100.0) * (A2 / 255.0);
					double tmpA = tmpA0 + (1 - tmpA0) * tmpA1;
					double tmpR = Math.round(R1 * tmpA0 / tmpA + R2 * (1 - tmpA0) * tmpA1 / tmpA);
					double tmpG = Math.round(G1 * tmpA0 / tmpA + G2 * (1 - tmpA0) * tmpA1 / tmpA);
					double tmpB = Math.round(B1 * tmpA0 / tmpA + B2 * (1 - tmpA0) * tmpA1 / tmpA);
					tmp.get(i).setA((byte) Math.round(tmpA * 255));
					tmp.get(i).setR((byte) tmpR);
					tmp.get(i).setG((byte) tmpG);
					tmp.get(i).setB((byte) tmpB);
				}
		return new Layer(tmp);
	}

	public ArrayList<Selection> getSelections()
	{
		return Selections;
	}

	public ArrayList<Layer> getLayers()
	{
		return Layers;
	}

	public boolean isSaved()
	{
		return Saved;
	}

}
