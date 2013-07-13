
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
	
	_Edit = {
		Ctrl = SupportCtrl.Edit,
		Name = t.Name,
		CName = t.CName,
		Type = SupportType[t.Type] or SupportType.String,
		Default = t.Default or "",
		Width1 = t.Width1 or 50,
		Width2 = t.Width2 or 50,
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
	if type(t.Checked) ~= "nil" and type(t.Checked) ~= "boolean" then
		error("Check:Checked must be boolean")
	end
	if type(t.Width1) ~= "nil" and type(t.Width1) ~= "number" then
		error("Check:Width1 must be number")
	end
	
	_Check = {
		Ctrl = SupportCtrl.Check,
		Name = t.Name,
		CName = t.CName,
		Checked = Checked or false,
		Width1 = t.Width1 or 50,
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
	
	_Static = {
		Ctrl = SupportCtrl.Static,
		Name = t.Name,
		Column = -1,
		Width1 = t.Width1 or 50,
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
	
	_Combobox = {
		Ctrl = SupportCtrl.Combobox,
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs,
		Width1 = t.Width1 or 50,
		Width2 = t.Width2 or 50,
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
	
	_CheckCombo = {
		Ctrl = SupportCtrl.CheckCombo,
		Name = t.Name,
		CName = t.CName,
		Confs = t.Confs,
		Width1 = t.Width1 or 50,
		Width2 = t.Width2 or 50,
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
	if type(t.Checked) ~= "nil" and type(t.Checked) ~= "boolean" then
		error("ComboItem:Checked  must be boolean")
	end
	
	_ComboItem = {
		Name = t.Name,
		Value = t.Value,
		Checked = t.Checked or false,
	}
	
	return _ComboItem
end