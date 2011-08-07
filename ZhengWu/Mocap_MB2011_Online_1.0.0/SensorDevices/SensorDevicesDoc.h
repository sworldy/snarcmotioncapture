// SensorDevicesDoc.h : interface of the CSensorDevicesDoc class
//


#pragma once


class CSensorDevicesDoc : public CDocument
{
protected: // create from serialization only
	CSensorDevicesDoc();
	DECLARE_DYNCREATE(CSensorDevicesDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSensorDevicesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


