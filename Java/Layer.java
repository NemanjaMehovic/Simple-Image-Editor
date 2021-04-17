package poop;

import java.util.ArrayList;

public class Layer
{
	private ArrayList<Pixel> Pixels;
	private double Transparency;
	private boolean Visible;
	private boolean Active;
	public static int Width = 0;
	public static int Height = 0;

	public Layer(ArrayList<Pixel> pixels)
	{
		this.Pixels = pixels;
		Visible = true;
		Active = true;
		Transparency = 100;
	}

	public boolean isVisible()
	{
		return Visible;
	}

	public void setVisible(boolean visible)
	{
		Visible = visible;
	}

	public boolean isActive()
	{
		return Active;
	}

	public void setActive(boolean active)
	{
		Active = active;
	}

	public double getTransparency()
	{
		return Transparency;
	}

	public void setTransparency(double transparency)
	{
		Transparency = transparency;
	}

	public ArrayList<Pixel> getPixels()
	{
		return Pixels;
	}

	public void Resize(int OldWidth)
	{
		ArrayList<Pixel> tmp = new ArrayList<Pixel>();
		int i = 0, j = 0;
		for (Pixel x : Pixels)
		{
			tmp.add(x);
			j++;
			if (j >= OldWidth)
			{
				while (j < Layer.Width)
				{
					tmp.add(new Pixel((byte) 0, (byte) 0, (byte) 0, (byte) 0));
					j++;
				}
				j = 0;
				i++;
			}
		}
		while (i < Layer.Height)
		{
			tmp.add(new Pixel((byte) 0, (byte) 0, (byte) 0, (byte) 0));
			j++;
			if (j >= Layer.Width)
			{
				i++;
				j = 0;
			}
		}
		Pixels = tmp;
	}

}
