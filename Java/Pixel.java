package poop;

public class Pixel
{

	private byte R;
	private byte G;
	private byte B;
	private byte A;

	public Pixel(byte r, byte g, byte b, byte a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	public Pixel(Pixel original)
	{
		R = original.R;
		G = original.G;
		B = original.B;
		A = original.A;
	}

	public byte getR()
	{
		return R;
	}

	public void setR(byte r)
	{
		R = r;
	}

	public byte getG()
	{
		return G;
	}

	public void setG(byte g)
	{
		G = g;
	}

	public byte getB()
	{
		return B;
	}

	public void setB(byte b)
	{
		B = b;
	}

	public byte getA()
	{
		return A;
	}

	public void setA(byte a)
	{
		A = a;
	}

}
