require "./BaseCfg"

Config = 
{
	MagicType = {
		Name = "技能",
		Excel = "./MagicTypeConfig.xls",
		Items = {
			Edit{Name = "ID", Column = 1, Type = "Int", Default = "-1"},
			Edit{Name = "Des", Column = 2, Type = "String", Default = "请填一段描述"},
			Combobox{
				Name = "攻击类型",
				Column = 3,
				Confs = {
					ComboItem{Name = "近程", Value = 0,},
					ComboItem{Name = "远程", Value = 1, Checked = true,},
				},
			},
			CheckCombo{
				Name = "可用职业",
				Column = 4,
				Confs = {
					ComboItem{Name = "战士", Value = 0,},
					ComboItem{Name = "弓箭手", Value = 1, Checked = true,},
					ComboItem{Name = "法师", Value = 2, Checked = true,},
				},
			},
			Check{Name = "受伤时打断", Column = 5, Checked = true},
		}
	},
	AuraEffectType = 
	{
		Name = "Buff",
		Excel = "./AuraEffectTypeConfig.xls",
		Items = {},
	},
}