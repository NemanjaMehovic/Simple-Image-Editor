package poop;

import java.util.ArrayList;

public class Selection
{
	private boolean Active;
	private ArrayList<Rectangle> Rectangles;
	private String Name;

	public Selection(ArrayList<Rectangle> rectangles, String name)
	{
		Rectangles = rectangles;
		Active = true;
		Name = name;
	}

	public boolean isActive()
	{
		return Active;
	}

	public void setActive(boolean active)
	{
		Active = active;
	}

	public ArrayList<Rectangle> getRectangles()
	{
		return Rectangles;
	}

	public String getName()
	{
		return Name;
	}

}
