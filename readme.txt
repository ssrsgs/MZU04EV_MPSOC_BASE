git管理工程文件使用如下脚本：

cd [get_property directory [current_project ]]			//CD切换到当前工作的文件
write_project_tcl {../Scripts/recreate_project.tcl}			//创建新建工程的TCL脚本	tcl中可设定工程的名字
write_bd_tcl {./Scripts/system_bd.tcl}				//创建生成bd文件的tcl脚本



使用时步骤如下：
	cd：切换目录：
				cd {E:\路径}
	执行tcl脚本	：
				source ./recreate_project.tcl
				source ./bd.tcl


create_project git_dem ./git_dem -part xc7a200tfbg484-1


create_project git_dem ./Work -part xczu4evsfvc784-2-i