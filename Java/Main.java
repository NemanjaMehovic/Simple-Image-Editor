package poop;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Canvas;
import java.awt.Checkbox;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dialog;
import java.awt.Dialog.ModalityType;
import java.awt.Dimension;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Label;
import java.awt.List;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.Panel;
import java.awt.TextField;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.ArrayList;

import javax.swing.JSlider;

import javafx.util.Pair;
import poop.Rectangle.Coordinate;

public class Main extends Frame
{
	private Picture Pic;
	private Panel LayerPanel;
	private Panel SelectionPanel;
	private Panel OperationPanel;
	private ImageCanvas PictureCanvas;
	private MenuBar menu;
	private Main window = null;
	private ArrayList<Operation> compositeOperation;

	private static class ImageCanvas extends Canvas
	{
		private Picture Pic = null;
		private int index = 0;
		private boolean createSelection = false;
		private ArrayList<Rectangle> Rectangles = new ArrayList<Rectangle>();
		private Coordinate Edge1, Edge2;

		public ImageCanvas()
		{
			super();
			addMouseListener(new MouseAdapter()
			{
				public void mousePressed(MouseEvent e)
				{
					if (!createSelection)
						return;
					Edge1 = new Coordinate(e.getX(), e.getY());
				}

				public void mouseReleased(MouseEvent e)
				{
					if (!createSelection)
						return;
					int x = e.getX(), y = e.getY();
					if (x > getWidth())
						x = getWidth();
					else if (x < 0)
						x = 0;
					if (y > getHeight())
						y = getHeight();
					else if (y < 0)
						y = 0;
					Edge2 = new Coordinate(x, y);
					Coordinate TopLeft = new Coordinate(0, 0);
					TopLeft.setX(Edge2.getX() < Edge1.getX() ? Edge2.getX() : Edge1.getX());
					TopLeft.setY(Edge2.getY() < Edge1.getY() ? Edge2.getY() : Edge1.getY());
					Coordinate BottomRight = new Coordinate(0, 0);
					BottomRight.setX(Edge2.getX() > Edge1.getX() ? Edge2.getX() : Edge1.getX());
					BottomRight.setY(Edge2.getY() > Edge1.getY() ? Edge2.getY() : Edge1.getY());
					TopLeft = scale(TopLeft);
					BottomRight = scale(BottomRight);
					Rectangles.add(new Rectangle(TopLeft, BottomRight));
					repaint();
				}
			});
		}

		@Override
		public void paint(Graphics g)
		{
			if (Pic == null)
			{
				g.clearRect(0, 0, getWidth(), getHeight());
				return;
			}
			Layer tmpLayer = Pic.CreatFinished(index);
			if (tmpLayer == null)
				return;
			BufferedImage tmpBufferedImage = new BufferedImage(Layer.Width, Layer.Height, BufferedImage.TYPE_INT_RGB);
			Formatter.Reverse(tmpLayer.getPixels(), Layer.Width, Layer.Height);
			for (int i = 0; i < Layer.Height; i++)
				for (int j = 0; j < Layer.Width; j++)
				{
					Pixel tmpPixel = tmpLayer.getPixels().get(i * Layer.Width + j);
					int rgb = tmpPixel.getA();
					rgb = rgb << 8;
					rgb = rgb + tmpPixel.getR();
					rgb = rgb << 8;
					rgb = rgb + tmpPixel.getG();
					rgb = rgb << 8;
					rgb = rgb + tmpPixel.getB();
					tmpBufferedImage.setRGB(j, i, rgb);
				}
			g.drawImage(tmpBufferedImage, 0, 0, getWidth(), getHeight(), null);
			if (Rectangles.size() == 0)
				return;
			for (Rectangle tmpRectangle : Rectangles)
			{
				float[] dash1 = { 2f };
				((Graphics2D) g).setStroke(new BasicStroke(1, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_ROUND, 1.0f, dash1, 2f));
				g.setColor(Color.WHITE);
				Coordinate edge = scaleBack(tmpRectangle.getTopLeft());
				g.drawRoundRect(edge.getX(), edge.getY(), (tmpRectangle.getWidth() * getWidth() / Layer.Width), (tmpRectangle.getHeight() * getHeight() / Layer.Height), 0, 0);
			}
		}

		public void setPic(Picture pic)
		{
			Pic = pic;
		}

		public void setIndex(int index)
		{
			this.index = index;
		}

		public boolean isCreateSelection()
		{
			return createSelection;
		}

		public void setCreateSelection(boolean createSelection)
		{
			this.createSelection = createSelection;
			Rectangles = new ArrayList<Rectangle>();
		}

		public ArrayList<Rectangle> getRectangles()
		{
			ArrayList<Rectangle> tmpArrayList = Rectangles;
			Rectangles = new ArrayList<Rectangle>();
			return tmpArrayList;
		}

		public void setRectangles(ArrayList<Rectangle> rectangles)
		{
			Rectangles = rectangles;
		}

		private Coordinate scale(Coordinate edge)
		{
			double ratioH = (double) Layer.Height / getHeight(), ratioW = (double) Layer.Width / getWidth();
			Coordinate tmpCoordinate = new Coordinate((int) (edge.getX() * ratioW), (int) ((getHeight() - edge.getY()) * ratioH));
			return tmpCoordinate;
		}

		private Coordinate scaleBack(Coordinate edge)
		{
			Coordinate tmpCoordinate = new Coordinate(edge.getX(), Layer.Height - edge.getY());
			double ratioH = (double) getHeight() / Layer.Height, ratioW = (double) getWidth() / Layer.Width;
			tmpCoordinate.setX((int) (tmpCoordinate.getX() * ratioW));
			tmpCoordinate.setY((int) (tmpCoordinate.getY() * ratioH));
			return tmpCoordinate;
		}
	}

	public Main()
	{
		super("bootleg photoshop");
		setSize(1000, 700);
		setLocationRelativeTo(null);
		window = this;
		ConfigLayerPanel();
		ConfigOperationPanel();
		ConfigSelectionPanel();
		ConfigMenu();

		PictureCanvas = new ImageCanvas();
		PictureCanvas.setSize(4 * this.getWidth() / 5, this.getHeight());
		this.add(PictureCanvas, BorderLayout.CENTER);
		this.add(LayerPanel, BorderLayout.EAST);

		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				if (Pic == null || Pic.isSaved())
					window.dispose();
				else
					window.ConfigExitDialog();
			}
		});
		setVisible(true);
	}

	private void ConfigMenu()
	{
		menu = new MenuBar();
		Menu file = new Menu("File");
		file.add("Load project or picture");
		file.add("Save project");
		file.add("Save finished");
		file.add("Exit");
		file.addActionListener(e -> {
			String tmpString = e.getActionCommand();
			FileDialog tmpFileDialog;
			Dialog tmpDialog;
			Formatter tmpFormatter;
			switch (tmpString)
			{
			case "Load project or picture":
				tmpFileDialog = new FileDialog(window, "Select picture or project", FileDialog.LOAD);
				tmpFileDialog.setLocationRelativeTo(window);
				tmpFileDialog.setModalityType(ModalityType.APPLICATION_MODAL);
				tmpFileDialog.setVisible(true);
				tmpDialog = ConfigErrorDialog("Pleas wait...", false);
				try
				{
					tmpFormatter = Formatter.Factory(tmpFileDialog.getFile().substring(tmpFileDialog.getFile().lastIndexOf('.')));
					Picture.DeleteInstance();
					CompositeOperation.GetCompositeOperations().clear();
					Pic = Picture.GetInstance(tmpFormatter, tmpFileDialog.getDirectory() + tmpFileDialog.getFile());
					window.EnableComponents();
					PictureCanvas.setIndex(0);
					PictureCanvas.setPic(Pic);
					PictureCanvas.repaint();
					window.ChangeStateLayer(0);
					window.remove(window.getComponentCount() - 1);
					window.add(LayerPanel, BorderLayout.EAST);
					tmpDialog.dispose();
				}
				catch (Exception e2)
				{
					Pic = null;
					Picture.DeleteInstance();
					tmpDialog.dispose();
					ConfigErrorDialog("Greska pri pravljenju projekta", true);
					PictureCanvas.setIndex(0);
					PictureCanvas.setPic(Pic);
					PictureCanvas.repaint();
					for (Component tmpComponent : LayerPanel.getComponents())
						tmpComponent.setEnabled(false);
					for (Component tmpComponent : SelectionPanel.getComponents())
						tmpComponent.setEnabled(false);
					for (Component tmpComponent : OperationPanel.getComponents())
						tmpComponent.setEnabled(false);
				}
				break;
			case "Save project":
				if (Pic == null)
					return;
				tmpFileDialog = new FileDialog(window, "Select where to save", FileDialog.SAVE);
				tmpFileDialog.setLocationRelativeTo(window);
				tmpFileDialog.setModalityType(ModalityType.APPLICATION_MODAL);
				tmpFileDialog.setVisible(true);
				tmpDialog = ConfigErrorDialog("Pleas wait...", false);
				try
				{
					Pic.Save(Formatter.Factory(".mn"), tmpFileDialog.getDirectory() + tmpFileDialog.getFile());
					tmpDialog.dispose();
				}
				catch (Exception e1)
				{
					tmpDialog.dispose();
					ConfigErrorDialog("Greska pri cuvanju projekta", true);
				}
				break;
			case "Save finished":
				if (Pic == null)
					return;
				tmpFileDialog = new FileDialog(window, "Select where to save", FileDialog.SAVE);
				tmpFileDialog.setLocationRelativeTo(window);
				tmpFileDialog.setModalityType(ModalityType.APPLICATION_MODAL);
				tmpFileDialog.setVisible(true);
				tmpDialog = ConfigErrorDialog("Pleas wait...", false);
				if (tmpFileDialog.getFile().lastIndexOf(".") != -1)
				{
					tmpFormatter = Formatter.Factory(tmpFileDialog.getFile().substring(tmpFileDialog.getFile().lastIndexOf(".")));
					if (tmpFormatter == null)
						tmpFormatter = Formatter.Factory(".bmp");
				}
				else
					tmpFormatter = Formatter.Factory(".bmp");
				try
				{
					Pic.Save(tmpFormatter, tmpFileDialog.getDirectory() + tmpFileDialog.getFile());
					tmpDialog.dispose();
				}
				catch (Exception e1)
				{
					tmpDialog.dispose();
					ConfigErrorDialog("Greska pri cuvanju slike", true);
				}
				break;
			case "Exit":
				if (Pic == null || Pic.isSaved())
					window.dispose();
				else
					window.ConfigExitDialog();
				break;
			}
		});
		Menu optionsMenu = new Menu("Options");
		optionsMenu.add("Layers");
		optionsMenu.add("Selections");
		optionsMenu.add("Operations");
		optionsMenu.addActionListener(e -> {
			window.remove(window.getComponentCount() - 1);
			String tmpString = e.getActionCommand();
			switch (tmpString)
			{
			case "Layers":
				window.add(LayerPanel, BorderLayout.EAST);
				PictureCanvas.setCreateSelection(false);
				PictureCanvas.setIndex(0);
				PictureCanvas.repaint();
				break;
			case "Selections":
				window.add(SelectionPanel, BorderLayout.EAST);
				PictureCanvas.setCreateSelection(false);
				PictureCanvas.setIndex(0);
				window.ChangeStateSelection(0);
				if (Pic != null && Pic.getSelections().size() != 0)
					PictureCanvas.setRectangles(Pic.getSelections().get(0).getRectangles());
				PictureCanvas.repaint();
				break;
			case "Operations":
				window.add(OperationPanel, BorderLayout.EAST);
				PictureCanvas.setCreateSelection(false);
				PictureCanvas.setIndex(0);
				PictureCanvas.repaint();
				if (Pic != null)
				{
					for (Component tmpComponent : OperationPanel.getComponents())
						tmpComponent.setEnabled(false);
					window.ChangeStateOperation(0);
					OperationPanel.getComponent(2).setEnabled(true);
					OperationPanel.getComponent(3).setEnabled(true);
					OperationPanel.getComponent(8).setEnabled(true);
					((Checkbox) OperationPanel.getComponent(2)).setState(false);
					((Checkbox) OperationPanel.getComponent(3)).setState(false);
				}
				break;
			}
			window.revalidate();
		});
		menu.add(file);
		menu.add(optionsMenu);
		this.setMenuBar(menu);
	}

	private void ConfigLayerPanel()
	{
		LayerPanel = new Panel();
		LayerPanel.setSize(this.getWidth() / 5, this.getHeight());
		LayerPanel.setPreferredSize(LayerPanel.getSize());
		LayerPanel.setBackground(Color.LIGHT_GRAY);
		List LayerList = new List(20, false);
		LayerList.setSize(LayerPanel.getWidth() * 2 / 3, LayerPanel.getHeight() / 2);
		LayerList.setPreferredSize(LayerList.getSize());
		LayerList.addItemListener(e -> {
			int index = ((List) LayerPanel.getComponent(1)).getSelectedIndex();
			window.ChangeStateLayer(index);
		});
		Label ListLabel = new Label("Layers:", Label.CENTER);
		LayerPanel.add(ListLabel);
		LayerPanel.add(LayerList);

		Checkbox Visible = new Checkbox("Visible", true);
		Visible.setPreferredSize(new Dimension(LayerPanel.getWidth() / 2 - 5, LayerPanel.getHeight() / 10));
		Visible.addItemListener(e -> {
			Checkbox tmpCheckbox = (Checkbox) LayerPanel.getComponent(2);
			int index = ((List) LayerPanel.getComponent(1)).getSelectedIndex();
			Pic.getLayers().get(index).setVisible(tmpCheckbox.getState());
			PictureCanvas.repaint();
		});
		Checkbox Active = new Checkbox("Active", true);
		Active.setPreferredSize(new Dimension(LayerPanel.getWidth() / 2 - 5, LayerPanel.getHeight() / 10));
		Active.addItemListener(e -> {
			Checkbox tmpCheckbox = (Checkbox) LayerPanel.getComponent(3);
			int index = ((List) LayerPanel.getComponent(1)).getSelectedIndex();
			Pic.getLayers().get(index).setActive(tmpCheckbox.getState());
			PictureCanvas.repaint();
		});
		LayerPanel.add(Visible);
		LayerPanel.add(Active);

		Label transparencyLabel = new Label("Transparency", Label.CENTER);
		LayerPanel.add(transparencyLabel);
		JSlider transparencySlider = new JSlider(JSlider.HORIZONTAL);
		transparencySlider.setMinorTickSpacing(5);
		transparencySlider.setMajorTickSpacing(25);
		transparencySlider.setPaintLabels(true);
		transparencySlider.setPaintTicks(true);
		transparencySlider.setSnapToTicks(true);
		transparencySlider.addChangeListener(e -> {
			JSlider tmpJSlider = (JSlider) LayerPanel.getComponent(5);
			int index = ((List) LayerPanel.getComponent(1)).getSelectedIndex();
			Pic.getLayers().get(index).setTransparency(tmpJSlider.getValue());
			PictureCanvas.repaint();
		});
		LayerPanel.setBackground(transparencySlider.getBackground());
		LayerPanel.add(transparencySlider);

		Label emptyLabel = new Label();
		emptyLabel.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 100));
		LayerPanel.add(emptyLabel);

		Button CreatEmptyLayer = new Button("New Layer");
		CreatEmptyLayer.setBackground(Color.WHITE);
		CreatEmptyLayer.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 20));
		CreatEmptyLayer.addActionListener(e -> {
			Pic.CreateNewLayer();
			((List) LayerPanel.getComponent(1)).add("Layer" + (Pic.getLayers().size() - 1));
			PictureCanvas.repaint();
		});
		Button CreatImageLayer = new Button("Load Image");
		CreatImageLayer.setBackground(Color.WHITE);
		CreatImageLayer.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 20));
		CreatImageLayer.addActionListener(e -> {
			FileDialog tmpFileDialog = new FileDialog(window, "Select picture", FileDialog.LOAD);
			tmpFileDialog.setLocationRelativeTo(window);
			tmpFileDialog.setModalityType(ModalityType.APPLICATION_MODAL);
			tmpFileDialog.setVisible(true);
			Formatter tmpFormatter = Formatter.Factory(tmpFileDialog.getFile().substring(tmpFileDialog.getFile().lastIndexOf('.')));
			try
			{
				if (tmpFileDialog.getFile().substring(tmpFileDialog.getFile().lastIndexOf('.')).equals(".mn"))
					throw new Exception();
				Pic.CreateNewLayer(tmpFormatter, tmpFileDialog.getDirectory() + tmpFileDialog.getFile());
				((List) LayerPanel.getComponent(1)).add("Layer" + (Pic.getLayers().size() - 1));
				PictureCanvas.repaint();
			}
			catch (Exception e1)
			{
				ConfigErrorDialog("Greska pri ucitavanju slike", true);
			}
		});
		Button DeleteLayer = new Button("Delete Layer");
		DeleteLayer.setBackground(Color.WHITE);
		DeleteLayer.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 20));
		DeleteLayer.addActionListener(e -> {
			if (Pic.getLayers().size() == 1)
			{
				ConfigErrorDialog("Greska mora postojati barem 1 layer", true);
				return;
			}
			int index = ((List) LayerPanel.getComponent(1)).getSelectedIndex();
			Pic.getLayers().remove(index);
			window.ChangeStateLayer(0);
			PictureCanvas.repaint();
		});

		LayerPanel.add(CreatEmptyLayer);
		emptyLabel = new Label();
		emptyLabel.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 100));
		LayerPanel.add(emptyLabel);
		LayerPanel.add(CreatImageLayer);
		emptyLabel = new Label();
		emptyLabel.setPreferredSize(new Dimension(LayerPanel.getWidth() * 3 / 4, LayerPanel.getHeight() / 100));
		LayerPanel.add(emptyLabel);
		LayerPanel.add(DeleteLayer);

		for (Component tmpComponent : LayerPanel.getComponents())
			tmpComponent.setEnabled(false);
	}

	private void ConfigSelectionPanel()
	{
		SelectionPanel = new Panel();
		SelectionPanel.setSize(this.getWidth() / 5, this.getHeight());
		SelectionPanel.setPreferredSize(SelectionPanel.getSize());
		SelectionPanel.setBackground(LayerPanel.getComponent(4).getBackground());
		List SelectionList = new List(20, false);
		SelectionList.setSize(SelectionPanel.getWidth() * 2 / 3, SelectionPanel.getHeight() / 2);
		SelectionList.setPreferredSize(SelectionList.getSize());
		SelectionList.addItemListener(e -> {
			if (Pic.getSelections().size() == 0)
				return;
			List tmpList = (List) SelectionPanel.getComponent(1);
			int index = tmpList.getSelectedIndex();
			window.ChangeStateSelection(index);
			PictureCanvas.setRectangles(Pic.getSelections().get(index).getRectangles());
			PictureCanvas.repaint();
		});
		Label ListLabel = new Label("Selections:", Label.CENTER);
		SelectionPanel.add(ListLabel);
		SelectionPanel.add(SelectionList);

		Checkbox Active = new Checkbox("Active", true);
		Active.setPreferredSize(new Dimension(SelectionPanel.getWidth() / 2 - 20, SelectionPanel.getHeight() / 10));
		Active.addItemListener(e -> {
			if (Pic.getSelections().size() == 0)
				return;
			if (!PictureCanvas.isCreateSelection())
			{
				List tmpList = (List) SelectionPanel.getComponent(1);
				int index = tmpList.getSelectedIndex();
				Checkbox tmpCheckbox = (Checkbox) SelectionPanel.getComponent(2);
				Pic.getSelections().get(index).setActive(tmpCheckbox.getState());
			}
		});
		Checkbox Create = new Checkbox("Enabel creation", false);
		Create.setPreferredSize(new Dimension(SelectionPanel.getWidth() / 2 + 5, SelectionPanel.getHeight() / 10));
		Create.addItemListener(e -> {
			Checkbox tmpCheckbox = (Checkbox) SelectionPanel.getComponent(3);
			PictureCanvas.setCreateSelection(tmpCheckbox.getState());
			TextField tmpField = (TextField) SelectionPanel.getComponent(4);
			Button tmpButton = (Button) SelectionPanel.getComponent(6);
			Button tmpButton2 = (Button) SelectionPanel.getComponent(8);
			List tmpList = (List) SelectionPanel.getComponent(1);
			tmpField.setEnabled(tmpCheckbox.getState());
			tmpButton.setEnabled(tmpCheckbox.getState());
			tmpList.setEnabled(!tmpCheckbox.getState());
			tmpButton2.setEnabled(!tmpCheckbox.getState());
			if (!tmpCheckbox.getState())
			{
				ChangeStateSelection(0);
				if (Pic.getSelections().size() != 0)
					PictureCanvas.setRectangles(Pic.getSelections().get(0).getRectangles());
				else
					PictureCanvas.setRectangles(new ArrayList<Rectangle>());
			}
			PictureCanvas.repaint();
		});
		SelectionPanel.add(Active);
		SelectionPanel.add(Create);

		TextField Name = new TextField("Selection name goes here");
		SelectionPanel.add(Name);

		Button CreateSelection = new Button("Create Selection");
		CreateSelection.setBackground(Color.WHITE);
		CreateSelection.setPreferredSize(new Dimension(SelectionPanel.getWidth() * 3 / 4, SelectionPanel.getHeight() / 20));
		CreateSelection.addActionListener(e -> {
			ArrayList<Rectangle> tmpArrayList = PictureCanvas.getRectangles();
			if (tmpArrayList.size() == 0)
			{
				ConfigErrorDialog("Nema pravougaonika", true);
				return;
			}
			String string = ((TextField) SelectionPanel.getComponent(4)).getText();
			if (!string.equals(""))
				Pic.getSelections().add(new Selection(tmpArrayList, string));
			else
				Pic.getSelections().add(new Selection(tmpArrayList, "Selection" + Pic.getSelections().size()));
			Pic.getSelections().get(Pic.getSelections().size() - 1).setActive(((Checkbox) SelectionPanel.getComponent(2)).getState());
			((List) SelectionPanel.getComponent(1)).add("Selection" + (Pic.getSelections().size() - 1));
			PictureCanvas.repaint();
		});
		Button DeleteSelection = new Button("Delete Selection");
		DeleteSelection.setBackground(Color.WHITE);
		DeleteSelection.setPreferredSize(new Dimension(SelectionPanel.getWidth() * 3 / 4, SelectionPanel.getHeight() / 20));
		DeleteSelection.addActionListener(e -> {
			if (Pic.getSelections().size() == 0)
				return;
			List tmpList = (List) SelectionPanel.getComponent(1);
			int index = tmpList.getSelectedIndex();
			Pic.getSelections().remove(index);
			ChangeStateSelection(0);
			if (Pic.getSelections().size() != 0)
				PictureCanvas.setRectangles(Pic.getSelections().get(0).getRectangles());
			else
				PictureCanvas.setRectangles(new ArrayList<Rectangle>());
			PictureCanvas.repaint();
		});
		Label emptyLabel = new Label();
		emptyLabel.setPreferredSize(new Dimension(SelectionPanel.getWidth() * 3 / 4, SelectionPanel.getHeight() / 25));
		SelectionPanel.add(emptyLabel);
		SelectionPanel.add(CreateSelection);
		emptyLabel = new Label();
		emptyLabel.setPreferredSize(new Dimension(SelectionPanel.getWidth() * 3 / 4, SelectionPanel.getHeight() / 25));
		SelectionPanel.add(emptyLabel);
		SelectionPanel.add(DeleteSelection);

		for (Component tmpComponent : SelectionPanel.getComponents())
			tmpComponent.setEnabled(false);
	}

	private void ConfigOperationPanel()
	{
		OperationPanel = new Panel();
		OperationPanel.setSize(this.getWidth() / 5, this.getHeight());
		OperationPanel.setPreferredSize(OperationPanel.getSize());
		OperationPanel.setBackground(LayerPanel.getComponent(4).getBackground());
		List OperationList = new List(17, false);
		OperationList.setSize(OperationPanel.getWidth() * 2 / 3, OperationPanel.getHeight() / 2);
		OperationList.setPreferredSize(OperationList.getSize());
		OperationList.addItemListener(e -> {
			if (!((Checkbox) OperationPanel.getComponent(2)).getState())
			{
				window.ChangeStateOperation(((List) OperationPanel.getComponent(1)).getSelectedIndex());
			}
			else
			{
				Dialog tmpDialog = new Dialog(window, ((List) OperationPanel.getComponent(1)).getSelectedItem());
				tmpDialog.setSize(300, 400);
				tmpDialog.setLocationRelativeTo(window);
				tmpDialog.setModal(false);
				List tmpList = new List(10);
				StringBuilder stringBuilder = new StringBuilder();
				CompositeOperation tmpOperation = CompositeOperation.GetCompositeOperations().get(((List) OperationPanel.getComponent(1)).getSelectedIndex());
				for (Operation tmp : tmpOperation.getOperations())
				{
					stringBuilder.append(tmp.GetXml() + " ");
					for (int i = 0; i < tmp.NumOfPar(); i++)
						stringBuilder.append(tmp.Get().getKey().get(i) + " ");
					if (tmp.NumOfStringPar() != 0)
						stringBuilder.append(tmp.Get().getValue());
					tmpList.add(stringBuilder.toString());
					stringBuilder = new StringBuilder();
				}
				tmpDialog.add(tmpList);
				tmpDialog.addWindowListener(new WindowAdapter()
				{
					public void windowClosing(WindowEvent e)
					{
						tmpDialog.dispose();
					}
				});
				tmpDialog.setVisible(true);
			}
		});
		Label ListLabel = new Label("Operations:", Label.CENTER);
		OperationPanel.add(ListLabel);
		OperationPanel.add(OperationList);

		Checkbox ShowComposite = new Checkbox("Show Composite", false);
		ShowComposite.setPreferredSize(new Dimension(OperationPanel.getWidth(), OperationPanel.getHeight() / 25));
		ShowComposite.addItemListener(e -> {
			Checkbox tmpCheckbox = (Checkbox) OperationPanel.getComponent(2);
			if (tmpCheckbox.getState())
			{
				List tmpList = (List) OperationPanel.getComponent(1);
				tmpList.removeAll();
				for (int i = 0; i < CompositeOperation.GetCompositeOperations().size(); i++)
					tmpList.add("CompositeOperation" + i);
				if (CompositeOperation.GetCompositeOperations().size() > 0)
					tmpList.select(0);
				for (int i = 3; i < OperationPanel.getComponents().length; i++)
					OperationPanel.getComponent(i).setEnabled(false);
			}
			else
				window.ChangeStateOperation(0);
			OperationPanel.getComponent(8).setEnabled(true);
			OperationPanel.getComponent(11).setEnabled(tmpCheckbox.getState());
			OperationPanel.getComponent(3).setEnabled(!tmpCheckbox.getState());
		});
		Checkbox CreateComposite = new Checkbox("Create Composite", false);
		CreateComposite.setPreferredSize(new Dimension(OperationPanel.getWidth(), OperationPanel.getHeight() / 25));
		CreateComposite.addItemListener(e -> {
			Checkbox tmpCheckbox = (Checkbox) OperationPanel.getComponent(3);
			compositeOperation = new ArrayList<Operation>();
			OperationPanel.getComponent(2).setEnabled(!tmpCheckbox.getState());
			OperationPanel.getComponent(8).setEnabled(!tmpCheckbox.getState());
			OperationPanel.getComponent(9).setEnabled(tmpCheckbox.getState());
			OperationPanel.getComponent(10).setEnabled(tmpCheckbox.getState());
		});
		OperationPanel.add(ShowComposite);
		OperationPanel.add(CreateComposite);

		TextField StrVar = new TextField("String Var");
		TextField Var1 = new TextField("Var1");
		TextField Var2 = new TextField("Var2");
		TextField Var3 = new TextField("Var3");
		OperationPanel.add(Var1);
		OperationPanel.add(Var2);
		OperationPanel.add(Var3);
		OperationPanel.add(StrVar);

		Button Execute = new Button("Execute");
		Execute.setBackground(Color.WHITE);
		Execute.setPreferredSize(new Dimension(OperationPanel.getWidth() * 3 / 4, OperationPanel.getHeight() / 20));
		Execute.addActionListener(e -> {
			Dialog tmpDialog = null;
			try
			{
				Checkbox tmpCheckbox = (Checkbox) OperationPanel.getComponent(2);
				CompositeOperation tmp;
				if (tmpCheckbox.getState())
					tmp = CompositeOperation.GetCompositeOperations().get(((List) OperationPanel.getComponent(1)).getSelectedIndex());
				else
				{
					try
					{
						ArrayList<Double> tmpArrayList = new ArrayList<Double>();
						String tmpString = "";
						int index = ((List) OperationPanel.getComponent(1)).getSelectedIndex();
						Operation tmpOperation = (Operation) Operation.GetOperations().get(index).getValue().clone();
						for (int i = 0; i < tmpOperation.NumOfPar(); i++)
							tmpArrayList.add(Double.parseDouble(((TextField) OperationPanel.getComponent(4 + i)).getText()));
						if (tmpOperation.NumOfStringPar() != 0)
							tmpString = ((TextField) OperationPanel.getComponent(7)).getText();
						tmpOperation.Set(new Pair<ArrayList<Double>, String>(tmpArrayList, tmpString));
						ArrayList<Operation> listaArrayList = new ArrayList<Operation>();
						listaArrayList.add(tmpOperation);
						tmp = new CompositeOperation(listaArrayList);
					}
					catch (Exception ex)
					{
						ConfigErrorDialog("Doslo je do greske", true);
						return;
					}
				}
				tmpDialog = ConfigErrorDialog("Please Wait...", false);
				tmp.SaveXml("tmp.fun");
				Pic.Save(Formatter.Factory(".mn"), ".\\finished.mn");
				String[] cmd = { "POOP lab1", "finished.mn", "tmp.fun" };
				Process p = Runtime.getRuntime().exec(cmd);
				p.waitFor();
				if (p.exitValue() != 0)
					throw new Exception();
				Picture.DeleteInstance();
				Pic = Picture.GetInstance(Formatter.Factory(".mn"), ".\\finished.mn");
				PictureCanvas.setPic(Pic);
				PictureCanvas.setIndex(0);
				PictureCanvas.repaint();
				File tmpfile = new File("finished.mn");
				tmpfile.delete();
				tmpfile = new File("tmp.fun");
				tmpfile.delete();
				tmpfile = new File("Layers");
				File[] allContents = tmpfile.listFiles();
				for (File file : allContents)
					file.delete();
				tmpfile.delete();
				tmpfile = new File("CompositeOperation");
				allContents = tmpfile.listFiles();
				for (File file : allContents)
					file.delete();
				tmpfile.delete();
				tmpDialog.dispose();
			}
			catch (Exception ex)
			{
				ConfigErrorDialog("Operacija nije uspesna", true);
				if (tmpDialog != null)
					tmpDialog.dispose();
			}
		});
		Button Add = new Button("Add Operation");
		Add.setBackground(Color.WHITE);
		Add.setPreferredSize(new Dimension(OperationPanel.getWidth() * 3 / 4, OperationPanel.getHeight() / 20));
		Add.addActionListener(e -> {
			try
			{
				ArrayList<Double> tmpArrayList = new ArrayList<Double>();
				String tmpString = "";
				int index = ((List) OperationPanel.getComponent(1)).getSelectedIndex();
				Operation tmpOperation = (Operation) Operation.GetOperations().get(index).getValue().clone();
				for (int i = 0; i < tmpOperation.NumOfPar(); i++)
					tmpArrayList.add(Double.parseDouble(((TextField) OperationPanel.getComponent(4 + i)).getText()));
				if (tmpOperation.NumOfStringPar() != 0)
					tmpString = ((TextField) OperationPanel.getComponent(7)).getText();
				tmpOperation.Set(new Pair<ArrayList<Double>, String>(tmpArrayList, tmpString));
				compositeOperation.add(tmpOperation);
			}
			catch (Exception ex)
			{
				ConfigErrorDialog("Operacija nije dodata", true);
			}
		});
		Button Create = new Button("Create Composite");
		Create.setBackground(Color.WHITE);
		Create.setPreferredSize(new Dimension(OperationPanel.getWidth() * 3 / 4, OperationPanel.getHeight() / 20));
		Create.addActionListener(e -> {
			if (compositeOperation.size() == 0)
			{
				ConfigErrorDialog("Nije ubacena nijedna operacija", true);
				return;
			}
			CompositeOperation tmpCompositeOperation = new CompositeOperation(compositeOperation);
			CompositeOperation.GetCompositeOperations().add(tmpCompositeOperation);
			compositeOperation = new ArrayList<Operation>();
		});
		Button Delete = new Button("Delete Composite");
		Delete.setBackground(Color.WHITE);
		Delete.setPreferredSize(new Dimension(OperationPanel.getWidth() * 3 / 4, OperationPanel.getHeight() / 20));
		Delete.addActionListener(e -> {
			if (CompositeOperation.GetCompositeOperations().size() == 0)
				return;
			List tmpList = (List) OperationPanel.getComponent(1);
			CompositeOperation.GetCompositeOperations().remove(tmpList.getSelectedIndex());
			tmpList.removeAll();
			for (int i = 0; i < CompositeOperation.GetCompositeOperations().size(); i++)
				tmpList.add("CompositeOperation" + i);
		});
		OperationPanel.add(Execute);
		OperationPanel.add(Add);
		OperationPanel.add(Create);
		OperationPanel.add(Delete);

		for (Component tmpComponent : OperationPanel.getComponents())
			tmpComponent.setEnabled(false);
	}

	private void ConfigExitDialog()
	{
		Dialog tmpDialog = new Dialog(window, "Project not saved", true);
		tmpDialog.addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				tmpDialog.dispose();
			}
		});
		Panel tmpPanel = new Panel();
		Button cancel = new Button("Cancel");
		cancel.setSize(50, 25);
		cancel.setPreferredSize(cancel.getSize());
		cancel.addActionListener(e -> {
			tmpDialog.dispose();
		});
		Button exit = new Button("Exit");
		exit.setSize(50, 25);
		exit.setPreferredSize(exit.getSize());
		exit.addActionListener(e -> {
			tmpDialog.dispose();
			window.dispose();
		});
		tmpPanel.add(exit);
		tmpPanel.add(cancel);
		tmpDialog.add(new Label("Project not saved", Label.CENTER), BorderLayout.CENTER);
		tmpDialog.add(tmpPanel, BorderLayout.SOUTH);
		tmpDialog.setSize(200, 100);
		tmpDialog.setResizable(false);
		tmpDialog.setLocationRelativeTo(window);
		tmpDialog.setVisible(true);
	}

	private Dialog ConfigErrorDialog(String s, boolean mode)
	{
		Dialog tmpDialog = new Dialog(window, "Info", mode);
		tmpDialog.addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				tmpDialog.dispose();
			}
		});
		tmpDialog.add(new Label(s, Label.CENTER));
		tmpDialog.setSize(250, 100);
		tmpDialog.setLocationRelativeTo(window);
		tmpDialog.setVisible(true);
		return tmpDialog;
	}

	private void EnableComponents()
	{
		for (Component tmpComponent : LayerPanel.getComponents())
			tmpComponent.setEnabled(true);
		for (Component tmpComponent : SelectionPanel.getComponents())
			tmpComponent.setEnabled(true);
		window.ChangeStateSelection(0);
		window.ChangeStateOperation(0);
		OperationPanel.getComponent(2).setEnabled(true);
		OperationPanel.getComponent(3).setEnabled(true);
		OperationPanel.getComponent(8).setEnabled(true);
		((Checkbox) OperationPanel.getComponent(2)).setState(false);
		((Checkbox) OperationPanel.getComponent(3)).setState(false);
		PictureCanvas.setCreateSelection(false);
	}

	private void ChangeStateLayer(int index)
	{
		if (index == 0)
		{
			List tmpList = (List) LayerPanel.getComponent(1);
			tmpList.removeAll();
			for (int i = 0; i < Pic.getLayers().size(); i++)
			{
				tmpList.add("Layer" + i);
			}
			tmpList.select(index);
		}
		Checkbox tmpCheckbox = (Checkbox) LayerPanel.getComponent(2);
		tmpCheckbox.setState(Pic.getLayers().get(index).isVisible());
		tmpCheckbox = (Checkbox) LayerPanel.getComponent(3);
		tmpCheckbox.setState(Pic.getLayers().get(index).isActive());
		JSlider tmpJSlider = (JSlider) LayerPanel.getComponent(5);
		tmpJSlider.setValue((int) Pic.getLayers().get(index).getTransparency());
	}

	private void ChangeStateSelection(int index)
	{
		if (Pic == null)
			return;
		List tmpList = (List) SelectionPanel.getComponent(1);
		tmpList.setEnabled(true);
		if (index == 0)
		{
			tmpList.removeAll();
			for (int i = 0; i < Pic.getSelections().size(); i++)
			{
				tmpList.add("Selection" + i);
			}
			tmpList.select(index);
		}
		Checkbox tmpCheckbox = (Checkbox) SelectionPanel.getComponent(3);
		tmpCheckbox.setState(false);
		TextField tmpField = (TextField) SelectionPanel.getComponent(4);

		tmpField.setEnabled(false);
		Button tmpButton = (Button) SelectionPanel.getComponent(6);
		Button tmpButton2 = (Button) SelectionPanel.getComponent(8);
		tmpButton.setEnabled(false);
		tmpButton2.setEnabled(true);
		tmpCheckbox = (Checkbox) SelectionPanel.getComponent(2);
		if (Pic.getSelections().size() != 0)
		{
			tmpCheckbox.setState(Pic.getSelections().get(index).isActive());
			tmpField.setText(Pic.getSelections().get(index).getName());
		}
		else
		{
			tmpCheckbox.setState(true);
			tmpField.setText("Selection name goes here");
		}
	}

	private void ChangeStateOperation(int index)
	{
		OperationPanel.getComponent(0).setEnabled(true);
		List tmpList = (List) OperationPanel.getComponent(1);
		tmpList.setEnabled(true);
		tmpList.removeAll();
		int i = 0;
		for (int j = 0; j < 3; j++)
			OperationPanel.getComponent(4 + j).setEnabled(false);
		for (Pair<String, Operation> tmpOperation : Operation.GetOperations())
		{
			tmpList.add(tmpOperation.getKey());
			if (i == index)
			{
				tmpList.select(index);
				for (int j = 0; j < tmpOperation.getValue().NumOfPar(); j++)
					OperationPanel.getComponent(4 + j).setEnabled(true);
				OperationPanel.getComponent(7).setEnabled(tmpOperation.getValue().NumOfStringPar() == 1);
			}
			i++;
		}
	}

	public static void main(String[] args)
	{
		new Main();
	}
}
