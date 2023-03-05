//
//NOTE: This sample source code belongs to John McNamara 
//      [libxlsxwriter](https://github.com/jmcnamara/libxlsxwriter#libxlsxwriter)
//      project. It can be found here: http://libxlsxwriter.github.io/working_with_charts.html
//
//      The libxlsxwriter library and all additional third party libraries are 
//      pulled in and build within the Visual-LIBXLSXWRITER-for-UEFI-Shell "subsolution"-hack
//      in the $(ProjectDir).
//      Same for Visual-LIBWIN32-for-UEFI "subsolution"-hack that provides Windows KERNEL32 
//      functions required to finish the build for UEFI.
// 
//      Check Project->Properties->Build Event->PreBuildEvent to see the magic.

#include "xlsxwriter.h"

int main() {

    lxw_workbook* workbook = workbook_new("chart_line.xlsx");
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart* chart;
    lxw_chart_series* series;

    /* Write some data for the chart. */
    worksheet_write_number(worksheet, 0, 0, 10, NULL);
    worksheet_write_number(worksheet, 1, 0, 40, NULL);
    worksheet_write_number(worksheet, 2, 0, 50, NULL);
    worksheet_write_number(worksheet, 3, 0, 20, NULL);
    worksheet_write_number(worksheet, 4, 0, 10, NULL);
    worksheet_write_number(worksheet, 5, 0, 50, NULL);

    /* Create a chart object. */
    chart = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Configure the chart. */
    series = chart_add_series(chart, NULL, "Sheet1!$A$1:$A$6");

    (void)series; /* Do something with series in the real examples. */

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("C1"), chart);

    printf("MS Excel file \"chart_line.xlsx\"\n");

    return workbook_close(workbook);
}
