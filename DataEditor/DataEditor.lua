require "./BaseCfg"

Config = 
{
	MagicType = {
		Name = "����",
		Excel = "./MagicTypeConfig.xls",
		Items = {
			Edit{Name = "ID", Column = 1, Type = "Int", Default = "-1"},
			Edit{Name = "Des", Column = 2, Type = "String", Default = "����һ������"},
			Combobox{
				Name = "��������",
				Column = 3,
				Confs = {
					ComboItem{Name = "����", Value = 0,},
					ComboItem{Name = "Զ��", Value = 1, Checked = true,},
				},
			},
			CheckCombo{
				Name = "����ְҵ",
				Column = 4,
				Confs = {
					ComboItem{Name = "սʿ", Value = 0,},
					ComboItem{Name = "������", Value = 1, Checked = true,},
					ComboItem{Name = "��ʦ", Value = 2, Checked = true,},
				},
			},
			Check{Name = "����ʱ���", Column = 5, Checked = true},
		}
	},
	AuraEffectType = 
	{
		Name = "Buff",
		Excel = "./AuraEffectTypeConfig.xls",
		Items = {},
	},
}