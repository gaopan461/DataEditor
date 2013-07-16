require "./BaseCfg"

PlatformConfig =
{
	PlatformType = SupportPlatform.Excel,
	ExcelHead = 2,
	ExcelData = 4,
}

EditorConfig = 
{
	{
		Name = "����",
		CName = "MagicType",
		Excel = "./MagicTypeConfig.xls",
		Key = "ID",
		Des = "Des",
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Default = "-1",},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������", Newline = true,},
			Static{Name = string.rep("-",100), Newline = true,},
			Combobox{
				Name = "��������",
				CName = "AttackType",
				Confs = {
					ComboItem{Name = "����", Value = 0,},
					ComboItem{Name = "Զ��", Value = 1, Checked = true,},
				},
			},
			CheckCombo{
				Name = "����ְҵ",
				CName = "Profession",
				Confs = {
					ComboItem{Name = "սʿ", Value = 0,},
					ComboItem{Name = "������", Value = 1, Checked = true,},
					ComboItem{Name = "��ʦ", Value = 2, Checked = true,},
				},
				Newline = true,
			},
			Check{Name = "����ʱ���", CName = "HitedBreak", Checked = true, Width1 = 200, Newline = true,},
		}
	},
	{
		Name = "Buff",
		CName = "AuraEffectType",
		Excel = "./AuraEffectTypeConfig.xls",
		Key = "ID",
		Des = "Des",
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Default = "-1",Newline = true,},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������",Newline = true,},
		},
	},
}