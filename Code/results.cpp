#include "results.h"

/* ==================================================================================== */
/*                           StatisticsLog create function                                */
/* ==================================================================================== */
/**
 * This function creates a csv document containing instanceId, valideSolution, nPlates,
 * totalGeoLoss and widthResidual attributes value
**/
void statisticsLog (void)
{
    unsigned int i;
    double waste, total_waste_percentage = 0.0;
    string temp;
    total_waste = 0.0;
    string statisticsPath = "logs\\" + file_idx + "_statistics.csv";
    statistics_file.open (statisticsPath.c_str());
    statistics_file << "instanceId;validSolution;nPlates;totalGeoLoss;widthResidual" << endl;

    statistics_file << file_idx << ";";

    if (constraint_error == 0)
        statistics_file << 1 << ";";
    else
        statistics_file << 0 << ";";

    statistics_file << plates_nbr << ";";
    for (i = 0; i < plates_nbr; i++)
    {
      if (i == plates_nbr-1) // If this is the last plate.
        waste = (double) plate[i].Getwaste()/((plate_w - plate[i].Getresidual().Getwidth())*plate_h/100); // Compute plate[i] wasted area percentage without taking into account residual area.
      else
        waste = (double) plate[i].Getwaste()/(plate_h*plate_w/100); // Compute plate[i] wasted area percentage.
      total_waste += waste;
    }
    total_waste_percentage = (double) total_waste/plates_nbr;
    statistics_file << total_waste_percentage << "%;" << plate[plates_nbr-1].Getresidual().Getwidth() << endl;
    statistics_file.close ();
}


/* ==================================================================================== */
/*                             Display Solution Results                                 */
/* ==================================================================================== */

/**
 * This function display a list of solution's violated constraints.
**/
void violatedConstraints (void)
{
    // If constraints violation occured.
    if (constraint_error != 0)
    {
        cout << endl    << "\t------------------------------------------------------------" << endl;
        cout            << "\t                  Violated Constraints List                 " << endl;
        cout            << "\t------------------------------------------------------------" << endl;

        log_file << endl    << "\t------------------------------------------------------------" << endl;
        log_file            << "\t                  Violated Constraints List                 " << endl;
        log_file            << "\t------------------------------------------------------------" << endl;

        // If Solution tree structure field set to 1
        if ((constraint_error & PARSE_ERROR_MASK) >> PARSE_ERROR_OFFSET)
        {
            cout << "\t- Solution Tree Structure Constraint violated" << endl;
            log_file << "\t- Solution Tree Structure Constraint violated" << endl;
        }

        // If Item production error field set to 1
        if ((constraint_error & ITEM_PRODUCTION_ERROR_MASK) >> ITEM_PRODUCTION_ERROR_OFFSET)
        {
            cout << "\t- Items production Constraint violated" << endl;
            log_file << "\t- Items production Constraint violated" << endl;
        }

        // If Defects superposing error field set to 1
        if ((constraint_error & DEFECTS_SUPERPOSING_ERROR_MASK) >> DEFECTS_SUPERPOSING_ERROR_OFFSET)
        {
            cout << "\t- Defects Superposing Constraint violated" << endl;
            log_file << "\t- Defects Superposing Constraint violated" << endl;
        }

        // If Nodes dimensions error field set to 1
        if ((constraint_error & DIMENSIONS_ERROR_MASK) >> DIMENSIONS_ERROR_OFFSET)
        {
            cout << "\t- Nodes Dimensions Constraint violated" << endl;
            log_file << "\t- Nodes Dimensions Constraint violated" << endl;
        }

        // If Sequence error field set to 1
        if ((constraint_error & SEQUENCE_ERROR_MASK) >> SEQUENCE_ERROR_OFFSET)
        {
            cout << "\t- Sequence Constraint violated" << endl;
            log_file << "\t- Sequence Constraint violated" << endl;
        }

        // If Identity error field set to 1
        if ((constraint_error & IDENTITY_ERROR_MASK) >> IDENTITY_ERROR_OFFSET)
        {
            cout << "\t- Identity Constraint violated" << endl;
            log_file << "\t- Identity Constraint violated" << endl;
        }

        cout            << "\t------------------------------------------------------------" << endl;
        log_file        << "\t------------------------------------------------------------" << endl;
    }
}

/**
 * This function show usage percentage of each plate
 * and a total computation of used/lost area of all used plates
 * Note: this function is accessible if 0 errors are reported by checker
 * verify function (dimensions, identity&sequence, item production and defects superposing)
**/
void displayPlatesAreaUsage (void)
{
    double useful = 0.0;
    double waste = 0.0;
    double total_useful_percentage = 0.0;
    double total_waste_percentage = 0.0;
    unsigned int all_plates_area = 0;
    double residual_percentage = 0.0;
    if (active_log)
        log_file << endl << "\t--- Display Plates Area Usage ---" << endl;
    else
        cout << endl << "\t--- Display Plates Area Usage ---" << endl;
    unsigned int i;
    total_waste = 0, total_useful = 0;
    if (active_log)
    {
        if (constraint_error)
        {
            log_file << "\tERROR -- Unable to verify Display Plates Area Usage due to INVALID SOLUTION" << endl;
            log_file << "\t--- End of Display Plates Area Usage ---" << endl;
            return;
        }
        log_file << "\t---------------------- Summary --------------------------" << endl;
        log_file << "\t|  Plates               |  Used Area    |  Wasted Area  |" << endl;
        log_file << "\t---------------------------------------------------------" << endl;
        for (i = 0; i < plates_nbr; i++)
        {
          if (i == plates_nbr-1) // If this is the last plate.
          {
            useful = (double) plate[i].Getuseful()/((plate_w - plate[i].Getresidual().Getwidth())*plate_h/100); // Compute plate[i] used area percentage without taking into account residual area.
            waste = (double) plate[i].Getwaste()/((plate_w - plate[i].Getresidual().Getwidth())*plate_h/100); // Compute plate[i] wasted area percentage without taking into account residual area.
          }
          else
          {
            useful = (double) plate[i].Getuseful()/(plate_h*plate_w/100); // Compute plate[i] used area percentage.
            waste = (double) plate[i].Getwaste()/(plate_h*plate_w/100); // Compute plate[i] wasted area percentage.
          }
          total_useful += plate[i].Getuseful();
          total_waste += plate[i].Getwaste();
          log_file << "\t|  Plate " << i << "\t\t|  " << useful << "%\t|  " << waste << "%\t|" << endl;
        }
        all_plates_area = (plate_w * plate_h) * (plates_nbr - 1) + (plate_w - plate[plates_nbr-1].Getresidual().Getwidth()) * plate_h;
        total_useful_percentage = (double) 100*total_useful/all_plates_area;
        total_waste_percentage = (double) 100*total_waste/all_plates_area;

        log_file << "\t---------------------------------------------------------" << endl;
        log_file << "\t|  Total (" << plates_nbr << " plate)\t|  " << total_useful_percentage << "%\t|  " << total_waste_percentage << "%\t|" << endl;
        log_file << "\t---------------------------------------------------------" << endl << endl;

        residual_percentage = plate[plates_nbr-1].Getresidual().Getwidth() * plate[plates_nbr-1].Getresidual().Getheight()/(plate_h*plate_w/100);

        log_file << "\t------------------------ Residual -----------------------" << endl;
        log_file << "\t|  Plate id             |  Width        |  Area %       |" << endl;
        log_file << "\t---------------------------------------------------------" << endl;
        log_file << "\t|  Plate " << plate[plates_nbr-1].Getplate_id() << "\t\t|  " << plate[plates_nbr-1].Getresidual().Getwidth() << "mm  \t|  " << residual_percentage << "%  \t|" << endl;
        log_file << "\t---------------------------------------------------------" << endl;
        log_file << "\t--- End of Display Plates Area Usage ---" << endl;
    }
    else
    {
        if (constraint_error)
        {
            cout  << "\tERROR -- Unable to Display Plates Area Usage due to INVALID SOLUTION" << endl;
            cout << "\t--- End of Display Plates Area Usage ---" << endl;
            return;
        }
        cout << "\t---------------------- Summary --------------------------" << endl;
        cout << "\t|  Plates               |  Used Area    |  Wasted Area  |" << endl;
        cout << "\t---------------------------------------------------------" << endl;
        for (i = 0; i < plates_nbr; i++)
        {
            if (i == plates_nbr-1) // If this is the last plate.
              {
                useful = (double) plate[i].Getuseful()/((plate_w - plate[i].Getresidual().Getwidth())*plate_h/100); // Compute plate[i] used area percentage without taking into account residual area.
                waste = (double) plate[i].Getwaste()/((plate_w - plate[i].Getresidual().Getwidth())*plate_h/100); // Compute plate[i] wasted area percentage without taking into account residual area.
              }
              else
              {
                useful = (double) plate[i].Getuseful()/(plate_h*plate_w/100); // Compute plate[i] used area percentage.
                waste = (double) plate[i].Getwaste()/(plate_h*plate_w/100); // Compute plate[i] wasted area percentage.
              }
              total_useful += plate[i].Getuseful();
              total_waste += plate[i].Getwaste();
              cout << "\t|  Plate " << i << "\t\t|  " << useful << "%\t|  " << waste << "%\t|" << endl;
        }
        all_plates_area = (plate_w * plate_h) * (plates_nbr - 1) + (plate_w - plate[plates_nbr-1].Getresidual().Getwidth()) * plate_h;
        total_useful_percentage = (double) 100*total_useful/all_plates_area;
        total_waste_percentage = (double) 100*total_waste/all_plates_area;

        cout << "\t---------------------------------------------------------" << endl;
        cout << "\t|  Total (" << plates_nbr << " plate)\t|  " << total_useful_percentage  << "%\t|  " << total_waste_percentage  << "%\t|" << endl;
        cout << "\t---------------------------------------------------------" << endl << endl;

        residual_percentage = plate[plates_nbr-1].Getresidual().Getwidth() * plate[plates_nbr-1].Getresidual().Getheight()/(plate_h*plate_w/100);

        cout << "\t------------------------ Residual -----------------------" << endl;
        cout << "\t|  Plate id             |  Width        |  Area %       |" << endl;
        cout << "\t---------------------------------------------------------" << endl;
        cout << "\t|  Plate " << plate[plates_nbr-1].Getplate_id() << "\t\t|  " << plate[plates_nbr-1].Getresidual().Getwidth() << "mm  \t|  " << residual_percentage << "%  \t|" << endl;
        cout << "\t---------------------------------------------------------" << endl;
        cout << "\t--- End of Display Plates Area Usage ---" << endl;
    }
}
