-- --------------------------------------------------------------------------
-- 枚举值定义
-- --------------------------------------------------------------------------

SupportType = {
	Int 	= 0,
	Float 	= 1,
	String 	= 2,
}

SupportCtrl = {
	Edit 		= 0,
	Check 		= 1,
	Static 		= 2,
	Combobox 	= 3,
	CheckCombo 	= 4,
}

SupportDB = 
{
	Excel 	= 0,
	Lua 	= 1,
	Python 	= 2,
}

-- --------------------------------------------------------------------------
-- 支持的DB定义
-- --------------------------------------------------------------------------

function ExcelDB(t)
	if type(t.Path) ~= "string" then
		error("ExcelDB:Require Path or Path must be string")
	end
	if type(t.Key) ~= "string" then
		error("ExcelDB:Require Key or Key must be string")
	end
	if type(Des) ~= "nil" and type(t.Des) ~= "string" then
		error("ExcelDB:Des must be string")
	end
	if type(t.HeadRow) ~= "number" then
		error("ExcelDB:Require HeadRow or HeadRow must be number")
	end
	if type(t.DataRow) ~= "number" then
		error("ExcelDB:Require DataRow or DataRow must be number")
	end
	
	_ExcelDB = {
		DBType = SupportDB.Excel,
		Path = t.Path,
		Key = t.Key,
		Des = t.Des or t.Key,
		HeadRow = t.HeadRow,
		DataRow = t.DataRow,
	}
	
	return _ExcelDB
end

-- --------------------------------------------------------------------------
-- 支持的控件定义
-- --------------------------------------------------------------------------

function Edit(t)
	if type(t.Name) ~= "string" then
		error("Edit:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("Edit:Require CName or CName must be string")
	end
	if type(t.Type) ~= "nil" and SupportType[t.Type] == nil then
		error("Edit:Type unknown")
	end
	if type(t.Default) ~= "nil" and type(t.Default) ~= "string" then
		error("Edit:Default must be string")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("Edit:Width1 must be number")
	end
	if type(t.Width2) ~= "nil" and type(t.Width2) ~= "number" then
		error("Edit:Width2 must be number")
	end
	if type(t.Newline) ~= "nil" and type(t.Newline) ~= "boolean" then
		error("Edit:Newline must be boolean")
	end
	
	_Edit = {
		Ctrl = SupportCtrl.Edit,
		Name = t.Name,
		CName = t.CName,
		Type = SupportType[t.Type] or SupportType.String,
		Default = t.Default or "",
		Width1 = t.Width1 or 100,
		Width2 = t.Width2 or 100,
		Newline = t.Newline or false,
	}
	
	return _Edit
end

function Check(t)
	if type(t.Name) ~= "string" then
		error("Check:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("Check:Require CName or CName must be string")
	end
	if type(t.Default) ~= "nil" and type(t.Default) ~= "boolean" then
		error("Check:Default must be boolean")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("Check:Width1 must be number")
	end
	if type(t.Newline) ~= "nil" and type(t.Newline) ~= "boolean" then
		error("Check:Newline must be boolean")
	end
	
	_Check = {
		Ctrl = SupportCtrl.Check,
		Name = t.Name,
		CName = t.CName,
		Default = t.Default or false,
		Width1 = t.Width1 or 100,
		Newline = t.Newline or false,
	}
	
	return _Check
end

function Static(t)
	if type(t.Name) ~= "string" then
		error("Static:Require Name or Name must be string")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("Static:Width1 must be number")
	end
	if type(t.Newline) ~= "nil" and type(t.Newline) ~= "boolean" then
		error("Static:Newline must be boolean")
	end
	
	_Static = {
		Ctrl = SupportCtrl.Static,
		Name = t.Name,
		CName = "",
		Width1 = t.Width1 or 800,
		Newline = t.Newline or false,
	}
	
	return _Static
end

function Combobox(t)
	if type(t.Name) ~= "string" then
		error("Combobox:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("Combobox:Require CName or CName must be string")
	end
	if type(t.Confs) ~= "table" then
		error("Combobox:Require Confs or Confs must be table")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("Combobox:Width1 must be number")
	end
	if type(t.Width2) ~= "nil" and type(t.Width2) ~= "number" then
		error("Combobox:Width2 must be number")
	end
	if type(t.Newline) ~= "nil" and type(t.Newline) ~= "boolean" then
		error("Combobox:Newline must be boolean")
	end
	
	_Combobox = {
		Ctrl = SupportCtrl.Combobox,
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs,
		Width1 = t.Width1 or 100,
		Width2 = t.Width2 or 100,
		Newline = t.Newline or false,
	}
	
	return _Combobox
end

function CheckCombo(t)
	if type(t.Name) ~= "string" then
		error("CheckCombo:Require Name or Name must be string")
	end
	if type(t.CName) ~= "string" then
		error("CheckCombo:Require CName or CName must be string")
	end
	if type(t.Confs) ~= "table" then
		error("CheckCombo:Require Confs or Confs must be table")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("CheckCombo:Width1 must be number")
	end
	if type(t.Width2) ~= "nil" and type(t.Width2) ~= "number" then
		error("CheckCombo:Width2 must be number")
	end
	if type(t.Newline) ~= "nil" and type(t.Newline) ~= "boolean" then
		error("CheckCombo:Newline must be boolean")
	end
	
	_CheckCombo = {
		Ctrl = SupportCtrl.CheckCombo,
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs,
		Width1 = t.Width1 or 100,
		Width2 = t.Width2 or 100,
		Newline = t.Newline or false,
	}
	
	return _CheckCombo
end

function ComboItem(t)
	if type(t.Name) ~= "string" then
		error("ComboItem:Require Name or Name must be string")
	end
	if type(t.Value) ~= "number" then
		error("ComboItem:Require Value or Value must be number")
	end
	if type(t.Default) ~= "nil" and type(t.Default) ~= "boolean" then
		error("ComboItem:Default  must be boolean")
	end
	
	_ComboItem = {
		Name = t.Name,
		Value = t.Value,
		Default = t.Default or false,
	}
	
	return _ComboItem
end