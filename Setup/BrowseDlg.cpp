// BrowseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "BrowseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Image list indexes
#define ILI_HARD_DISK       0
#define ILI_FLOPPY          1
#define ILI_CD_ROM          2
#define ILI_NET_DRIVE       3
#define ILI_RAM_DRIVE       4
#define ILI_CLOSED_FOLDER   5
#define ILI_OPEN_FOLDER     6

/////////////////////////////////////////////////////////////////////////////
// CBrowseDlg dialog


CBrowseDlg::CBrowseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowseDlg)
	m_path = _T("");
	//}}AFX_DATA_INIT
}


void CBrowseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowseDlg)
	DDX_Control(pDX, IDC_DIRECTORY_TREE, m_DirectoryTree);
	DDX_Text(pDX, IDC_PATH, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowseDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowseDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DIRECTORY_TREE, OnSelchangedDirectoryTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_DIRECTORY_TREE, OnItemexpandingDirectoryTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowseDlg message handlers
BOOL CBrowseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/* Show the initial directory */
	UpdateData(FALSE);

	/* Load the image list */
	m_imglTreeImages.Create(IDR_DRIVEIMAGES, 16, 1, RGB (255, 0, 255));
	m_DirectoryTree.SetImageList(&m_imglTreeImages, TVSIL_NORMAL);

	/* Get the disk drive bit mask */
	DWORD dwDriveList = ::GetLogicalDrives ();

	/* Show the drives */
	int nPos = 0;
	CString strDrive = "?:\\";
	while (dwDriveList)
	{
		if (dwDriveList & 1) 
		{
			/* This drive exists. Add it to the tree */
			strDrive.SetAt (0, 'A' + nPos);

			HTREEITEM hItem;

			UINT nType = ::GetDriveType((LPCTSTR)strDrive);
			UINT nDrive = (UINT)strDrive[0] - 0x41;

			switch (nType) 
			{
			case DRIVE_REMOVABLE:
				hItem = m_DirectoryTree.InsertItem(strDrive, ILI_FLOPPY, ILI_FLOPPY);
				m_DirectoryTree.InsertItem("", 0, 0, hItem); //Adding dummy child
				break;

			case DRIVE_FIXED:
				hItem = m_DirectoryTree.InsertItem(strDrive, ILI_HARD_DISK, ILI_HARD_DISK);
				m_DirectoryTree.InsertItem("", 0, 0, hItem); //Adding dummy child
				break;

			case DRIVE_REMOTE:
				hItem = m_DirectoryTree.InsertItem(strDrive, ILI_NET_DRIVE, ILI_NET_DRIVE);
				m_DirectoryTree.InsertItem("", 0, 0, hItem); //Adding dummy child
				break;

			case DRIVE_CDROM:
				hItem = m_DirectoryTree.InsertItem(strDrive, ILI_CD_ROM, ILI_CD_ROM);
				m_DirectoryTree.InsertItem("", 0, 0, hItem); //Adding dummy child
				break;

			case DRIVE_RAMDISK:
				hItem = m_DirectoryTree.InsertItem(strDrive, ILI_RAM_DRIVE, ILI_RAM_DRIVE);
				m_DirectoryTree.InsertItem("", 0, 0, hItem); //Adding dummy child
				break;

			//case DRIVE_NO_ROOT_DIR:
			//case DRIVE_UNKNOWN:
			default:
				//Ignore
				break;
			}
		}
		dwDriveList >>= 1;
		nPos++;
	}



	/* Expand the tree to show the currently selected item */
	CString szPath=m_path;
	if (szPath.Right(1)!="\\") szPath+='\\';

	CString szNode;
	HTREEITEM hParentItem=TVI_ROOT;
	HTREEITEM hCurrentItem;
	int iLength;
	for (;;)
	{
		/* Find the current tree item that matches the selected path */
		hCurrentItem=m_DirectoryTree.GetChildItem(hParentItem);
		while (hCurrentItem!=NULL)
		{
			/* Go through each node */
			szNode=m_DirectoryTree.GetItemText(hCurrentItem);

			iLength=szNode.GetLength();
			if (szNode[iLength-1]!='\\')
			{
				szNode+='\\';
				iLength++;
			}

			if (szNode.CompareNoCase(szPath.Left(iLength))==0)
				break;

			/* No match, so go to next child */
			hCurrentItem=m_DirectoryTree.GetNextSiblingItem(hCurrentItem);
		}

		/* Check for no match */
		if (hCurrentItem==NULL) break;

		/* Remove this item from the string and see if we are done */
		szPath=szPath.Mid(iLength);
		if (szPath.IsEmpty())
		{
			m_DirectoryTree.SelectItem(hCurrentItem);
			break;
		}

		/* This node is a match, so expand it */
		if (!m_DirectoryTree.Expand(hCurrentItem,TVE_EXPAND))
			break;

		/* Go down to the next level */
		hParentItem=hCurrentItem;
	}

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
}




void CBrowseDlg::OnSelchangedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	/* Get the handle of the selected item */
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	/* Get the full path name */
	m_path.Empty();
	CString szTmp;

	while (hItem != NULL) 
	{
		szTmp = m_DirectoryTree.GetItemText(hItem);
		if ((szTmp.Right (1) != "\\")&&(!m_path.IsEmpty()))
			szTmp += "\\";

		m_path = szTmp + m_path;

		hItem = m_DirectoryTree.GetParentItem (hItem);
	}

	UpdateData(FALSE);
	
	*pResult = 0;
}


void CBrowseDlg::OnItemexpandingDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hNewItem;

	/* Set default result */
	*pResult = 0;

	/* Get the handle of the expanding item */
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	/* Delete all the children for this item */
	HTREEITEM hChild;
	while ((hChild = m_DirectoryTree.GetChildItem (hItem))!=NULL)
	{
		m_DirectoryTree.DeleteItem (hChild);
	}

	/* If we are collapsing, add dummy node and quit */
	if (pNMTreeView->action!=TVE_EXPAND)
	{
		/* Add a dummy child */
		m_DirectoryTree.InsertItem("", 0, 0, hItem);
		return;
	}

	/* Add the directory items for this node */
	/* First, get the path name */
	CString szTmp;
	CString szPath;

	szPath="*.*";
	hNewItem=hItem;
	while (hNewItem != NULL) 
	{
		szTmp = m_DirectoryTree.GetItemText(hNewItem);
		if (szTmp.Right (1) != "\\") szTmp += "\\";

		szPath = szTmp + szPath;

		hNewItem = m_DirectoryTree.GetParentItem (hNewItem);
	}

	/* Now, do a directory search */
	CFileFind ffSearch;
	BOOL bFoundOne=FALSE;

	if (ffSearch.FindFile(szPath))
	{
		//FSO 4-14-99 Fixing bug that did not show last directory
		//while (ffSearch.FindNextFile())
		BOOL bMore;
		for (;;)
		{
			bMore=ffSearch.FindNextFile();
			if ((ffSearch.IsDirectory())&&(!ffSearch.IsDots()))
			{
				/* Add this folder to the tree */
				hNewItem = m_DirectoryTree.InsertItem(ffSearch.GetFileName(), ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem);
				m_DirectoryTree.InsertItem("", 0, 0, hNewItem); //Adding dummy child
				bFoundOne=TRUE;
			}
			if (!bMore) break;
		}
	}
	ffSearch.Close();
	
	/* If did not find any, don't allow expand */
	if (!bFoundOne) *pResult=TRUE;
	else m_DirectoryTree.SortChildren(hItem);
}

