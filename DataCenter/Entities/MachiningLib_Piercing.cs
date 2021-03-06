//------------------------------------------------------------------------------
// <auto-generated>
//     此代码已从模板生成。
//
//     手动更改此文件可能导致应用程序出现意外的行为。
//     如果重新生成代码，将覆盖对此文件的手动更改。
// </auto-generated>
//------------------------------------------------------------------------------

namespace DataCenter.Entities
{
    using System;
    using System.Collections.Generic;
    
    public partial class MachiningLib_Piercing
    {
        public string Id { get; set; }
        public short ENO { get; set; }
        public string MachiningType { get; set; }
        public string MaterialType { get; set; }
        public double MaterialThickness { get; set; }
        public double FocalDistance { get; set; }
        public double FocalPosition { get; set; }
        public string CutterType { get; set; }
        public double CutterDiameter { get; set; }
        public short Piercing_Power { get; set; }
        public short Piercing_Freq { get; set; }
        public short Piercing_Duty { get; set; }
        public short Piercing_I_Freq { get; set; }
        public short Piercing_I_Duty { get; set; }
        public double Piercing_Step_T { get; set; }
        public short Piercing_Step_Sum { get; set; }
        public double Piercing_Pier_T { get; set; }
        public double Piercing_G_Press { get; set; }
        public short Piercing_G_Kind { get; set; }
        public double Piercing_G_Time { get; set; }
        public double Piercing_Def_Pos { get; set; }
        public double Piercing_Def_Pos2 { get; set; }
        public string Piercing_Gap_Axis { get; set; }
        public string Piercing_Def_Pos2_Dec { get; set; }
        public string Piercing_Reserve_Info_1 { get; set; }
        public string Piercing_Reserve_Info_2 { get; set; }
        public string Piercing_Reserve_Info_3 { get; set; }
    }
}
