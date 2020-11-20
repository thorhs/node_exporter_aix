#!/bin/ksh

struct_name=$1
input="data_sources/$1.multiple"
output="$2"

sed -n 1p $input | read name_field id_struct label
sed -n 2p $input | read initializer

if [ -z "$label" ]; then
	label="DELETE"
fi
sed "s/STRUCT_NAME/$struct_name/g; s/NAME_FIELD/$name_field/g; s/ID_STRUCT/$id_struct/g; s/INITIALIZER/$initializer/g; s/LABEL/$label/g; /DELETE/d"  templates/generate_multiple.template > $output

tail -n +3  $input | sed "s/\"/'/g" | while read type name desc; do
	echo "\toutput_${struct_name}_stat_mode(response, static_labels, \"aix_${struct_name}_${name}\", \"${type}\", \"${desc}\", ${struct_name}s, ${struct_name}_count, [](perfstat_${struct_name}_t& ${struct_name}) { return ${struct_name}.${name}; });" >> $output
done

echo "}" >> $output
