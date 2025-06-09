 printf("| %-8s | %-20s | %-3d | %-6s | %-15s | %-15s | %-15s | %-15s | %-6s | %-19s |\n",
                patients[i].patient_id, patients[i].p_name, patients[i].p_age, patients[i].p_gender,
                patients[i].p_blood_group, patients[i].p_disease, patients[i].p_contact_num,
                patients[i].is_minor ? (strlen(patients[i].guardian_cnic) ? patients[i].guardian_cnic : "-") : patients[i].p_cnic,
                patients[i].is_minor ? "Minor" : "Adult", reg_time_str);
        }