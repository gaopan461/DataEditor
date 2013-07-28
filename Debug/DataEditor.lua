require "./BaseCfg"

EditorConfig = 
{
	{
		Name = "技能",
		CName = "MagicType",
		DB = ExcelDB{Path = "C:\\Users\\Public\\Documents\\TestProject\\DataEditor\\Debug\\MagicTypeConfig.xls", Key = "ID", Des = "Des", HeadRow = 2, DataRow = 5,},
		Layers = {"_Sheet","AttackType","Profession","Des"},
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int",},
			Edit{Name = "描述", CName = "Des", Type = "String", Default = "请填一段描述", Newline = true,},
			Static{Name = string.rep("-",100), Newline = true,},
			Combobox{
				Name = "攻击类型",
				CName = "AttackType",
				Confs = {
					ComboItem{Name = "近程", Value = 0,},
					ComboItem{Name = "远程", Value = 1, Default = true,},
				},
			},
			CheckCombo{
				Name = "可用职业",
				CName = "Profession",
				Confs = {
					ComboItem{Name = "战士", Value = 0,},
					ComboItem{Name = "弓箭手", Value = 1, Default = true,},
					ComboItem{Name = "法师", Value = 2, Default = true,},
				},
				Newline = true,
			},
			Check{Name = "受伤时打断", CName = "HitedBreak", Default = true, Width1 = 200, Newline = true,},
		}
	},
	{
		Name = "Buff",
		CName = "AuraEffectType",
		DB = ExcelDB{Path = "C:\\Users\\Public\\Documents\\TestProject\\DataEditor\\Debug\\AuraEffectTypeConfig.xls", Key = "ID", Des = "Des", HeadRow = 2, DataRow = 5,},
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Newline = true,},
			Edit{Name = "描述", CName = "Des", Type = "String", Default = "请填一段描述",Newline = true,},
		},
	},
}