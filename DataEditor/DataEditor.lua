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
		Name = "技能",
		CName = "MagicType",
		Excel = "./MagicTypeConfig.xls",
		Key = "ID",
		Des = "Des",
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Default = "-1",},
			Edit{Name = "描述", CName = "Des", Type = "String", Default = "请填一段描述", Newline = true,},
			Static{Name = string.rep("-",100), Newline = true,},
			Combobox{
				Name = "攻击类型",
				CName = "AttackType",
				Confs = {
					ComboItem{Name = "近程", Value = 0,},
					ComboItem{Name = "远程", Value = 1, Checked = true,},
				},
			},
			CheckCombo{
				Name = "可用职业",
				CName = "Profession",
				Confs = {
					ComboItem{Name = "战士", Value = 0,},
					ComboItem{Name = "弓箭手", Value = 1, Checked = true,},
					ComboItem{Name = "法师", Value = 2, Checked = true,},
				},
				Newline = true,
			},
			Check{Name = "受伤时打断", CName = "HitedBreak", Checked = true, Width1 = 200, Newline = true,},
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
			Edit{Name = "描述", CName = "Des", Type = "String", Default = "请填一段描述",Newline = true,},
		},
	},
}