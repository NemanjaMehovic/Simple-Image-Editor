package poop;

public class Rectangle
{

	private Coordinate TopLeft;
	private Coordinate BottomRight;
	private int Width;
	private int Height;

	public static class Coordinate
	{
		private int x;
		private int y;

		public Coordinate(int x, int y)
		{
			this.x = x;
			this.y = y;
		}

		public int getX()
		{
			return x;
		}

		public void setX(int x)
		{
			this.x = x;
		}

		public int getY()
		{
			return y;
		}

		public void setY(int y)
		{
			this.y = y;
		}
	}

	public Rectangle(Coordinate topLeft, Coordinate bottomRight)
	{
		TopLeft = topLeft;
		BottomRight = bottomRight;
		Width = bottomRight.x - topLeft.x;
		Height = topLeft.y - bottomRight.y;
	}

	public Coordinate getTopLeft()
	{
		return TopLeft;
	}

	public Coordinate getBottomRight()
	{
		return BottomRight;
	}

	public int getWidth()
	{
		return Width;
	}

	public int getHeight()
	{
		return Height;
	}

}
