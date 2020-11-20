#!/bin/ksh

struct_name=$1
output="$2"

sed "s/STRUCT_NAME/$struct_name/g" templates/generate_total.template > $output

cat data_sources/$struct_name.total | sed "s/\"/'/g" | while read type name desc; do
	echo "\toutput_${struct_name}_stat(response, static_labels, \"aix_${struct_name}_${name}\", \"${type}\", \"${desc}\", ${struct_name}, [](perfstat_${struct_name}_total_t& ${struct_name}) { return ${struct_name}.${name}; });" >> $output
done

echo "}" >> $output
