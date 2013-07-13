require "./BaseCfg"

Configs =
{
	DBType = SupportDB.Excel,
	ExcelHead = 2,
}

Datas = 
{
	MagicType = {
		Name = "����",
		Excel = "./MagicTypeConfig.xls",
		Key = "ID",
		Des = "Des",
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Default = "-1"},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������"},
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
			},
			Check{Name = "����ʱ���", CName = "HitedBreak", Checked = true},
		}
	},
	AuraEffectType = 
	{
		Name = "Buff",
		Excel = "./AuraEffectTypeConfig.xls",
		Key = "ID",
		Des = "Des",
		Items = {
			Edit{Name = "ID", CName = "ID", Type = "Int", Default = "-1"},
			Edit{Name = "����", CName = "Des", Type = "String", Default = "����һ������"},
		},
	},
}