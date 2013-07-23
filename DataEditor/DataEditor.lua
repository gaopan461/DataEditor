require "./BaseCfg"

EditorConfig = 
{
	{
		Name = "����",
		CName = "MagicType",
		DB = ExcelDB{Path = "./MagicTypeConfig.xls", Key = "ID", Des = "Des", HeadRow = 2, DataRow = 4,},
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int",},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������", Newline = true,},
			Static{Name = string.rep("-",100), Newline = true,},
			Combobox{
				Name = "��������",
				CName = "AttackType",
				Confs = {
					ComboItem{Name = "����", Value = 0,},
					ComboItem{Name = "Զ��", Value = 1, Default = true,},
				},
			},
			CheckCombo{
				Name = "����ְҵ",
				CName = "Profession",
				Confs = {
					ComboItem{Name = "սʿ", Value = 0,},
					ComboItem{Name = "������", Value = 1, Default = true,},
					ComboItem{Name = "��ʦ", Value = 2, Default = true,},
				},
				Newline = true,
			},
			Check{Name = "����ʱ���", CName = "HitedBreak", Default = true, Width1 = 200, Newline = true,},
		}
	},
	{
		Name = "Buff",
		CName = "AuraEffectType",
		DB = ExcelDB{Path = "./AuraEffectTypeConfig.xls", Key = "ID", Des = "Des", HeadRow = 2, DataRow = 4,},
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Newline = true,},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������",Newline = true,},
		},
	},
}