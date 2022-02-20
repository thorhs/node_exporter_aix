#!/bin/ksh

struct_name=$1
input="data_sources/$1.multiple"
output="$2"

sed -n 1p $input | read name_field id_struct labels
sed -n 2p $input | read initializer

if [ -z "$label"s ]; then
	label="DELETE"
fi
/opt/freeware/bin/sed -n "0,/LABEL/{s/STRUCT_NAME/$struct_name/g; s/NAME_FIELD/$name_field/g; s/ID_STRUCT/$id_struct/g; s/INITIALIZER/$initializer/g; /LABEL/d; p}"  templates/generate_multiple.template > $output

for label in $labels; do
	echo $label | sed 's/:/ /' | read label format
	if [ -z "$format" ]; then
		format_start=""
		format_end=""
	else
		format_start="std::$format << "
		format_end="<< std::dec "
	fi
	/opt/freeware/bin/sed -n "/LABEL/{s/LABEL/$label/g; s/STRUCT_NAME/$struct_name/g; s/FORMAT_START/$format_start/; s/FORMAT_END/$format_end/; p}"  templates/generate_multiple.template >> $output
done

/opt/freeware/bin/sed -n "/LABEL/,\${s/STRUCT_NAME/$struct_name/g; s/NAME_FIELD/$name_field/g; s/ID_STRUCT/$id_struct/g; s/INITIALIZER/$initializer/g; /LABEL/d; p}"  templates/generate_multiple.template >> $output

tail -n +3  $input | sed "s/\"/'/g" | while read type name desc; do
	echo "\toutput_${struct_name}_stat_mode(response, static_labels, \"aix_${struct_name}_${name}\", \"${type}\", \"${desc}\", ${struct_name}s, ${struct_name}_count, [](perfstat_${struct_name}_t& ${struct_name}) { return ${struct_name}.${name}; });" >> $output
done

echo "}" >> $output
